#if defined(_WIN32)
#include "input/windows.hh"
#include "input/_utils.hh"
#include "input/key.hh"
#define WIN32_LEAN_AND_MEAN
#define VC_EXTRALEAN
#include <Windows.h>
#include <commctrl.h>
#include <spdlog/spdlog.h>
#include <utility>
namespace {
  using namespace aer;


  struct CallbackCtx {
    rigtorp::SPSCQueue<InputEvent> *queue;
    const std::chrono::steady_clock *clock;
    const std::atomic<double> *epoch;
    bool kb_state[256] =
        {}; // keyboard state
            // we have to keep track of whether or not the keys are pressed to
            // prevent queuing auto-repeat events because WM_INPUT does not tell
            // you if a press event is caused by auto-repeat. because the API
            // sucks
  };


  LRESULT CALLBACK wnd_proc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
    // get context
    auto *ctx = (CallbackCtx *)GetWindowLongPtrW(hwnd, GWLP_USERDATA);
    if (!ctx || ctx->queue == nullptr || ctx->clock == nullptr ||
        ctx->epoch == nullptr) {
      return DefWindowProcW(hwnd, msg, wparam, lparam);
    }

    // timestamp
    auto t = ctx->clock->now();
    double timestamp =
        std::chrono::duration<double, std::milli>(t.time_since_epoch())
            .count() -
        *ctx->epoch;

    // process msg
    if (msg != WM_INPUT) {
      return DefWindowProcW(hwnd, msg, wparam, lparam);
    }

    // get raw data
    RAWINPUT raw;
    HRAWINPUT handle = (HRAWINPUT)lparam;
    UINT size = sizeof(RAWINPUT);
    if (GetRawInputData(handle, RID_INPUT, &raw, &size,
                        sizeof(RAWINPUTHEADER)) == (UINT)-1) {
      return DefWindowProcW(hwnd, msg, wparam, lparam);
    }

    switch (raw.header.dwType) {

    // KEYBOARD EVENT
    case RIM_TYPEKEYBOARD: {
      USHORT &vkey = raw.data.keyboard.VKey;
      if (raw.data.keyboard.Flags == RI_KEY_MAKE) { // key press
        if (!ctx->kb_state[vkey]) {
          ctx->queue->try_push(InputEvent{
              InputControllerKind::KEY,
              InputEventKind::PRESSED,
              vkey,
              timestamp,
          });
          ctx->kb_state[vkey] = true;
        }
      } else if (raw.data.keyboard.Flags == RI_KEY_BREAK) { // key release
        ctx->queue->try_push(InputEvent{
            InputControllerKind::KEY,
            InputEventKind::RELEASED,
            vkey,
            timestamp,
        });
        ctx->kb_state[vkey] = false;
      }
    } break;

    // mouse event
    case RIM_TYPEMOUSE: {
      USHORT &button_flags = raw.data.mouse.usButtonFlags;
      USHORT &button_data = raw.data.mouse.usButtonData;
      switch (button_flags) {
      case RI_MOUSE_LEFT_BUTTON_DOWN:
        ctx->queue->try_push(InputEvent{
            InputControllerKind::MOUSE,
            InputEventKind::PRESSED,
            (unsigned int)Button::LEFT,
            timestamp,
        });
        break;
      case RI_MOUSE_LEFT_BUTTON_UP:
        ctx->queue->try_push(InputEvent{
            InputControllerKind::MOUSE,
            InputEventKind::RELEASED,
            (unsigned int)Button::LEFT,
            timestamp,
        });
        break;
      case RI_MOUSE_RIGHT_BUTTON_DOWN:
        ctx->queue->try_push(InputEvent{
            InputControllerKind::MOUSE,
            InputEventKind::PRESSED,
            (unsigned int)Button::RIGHT,
            timestamp,
        });
        break;
      case RI_MOUSE_RIGHT_BUTTON_UP:
        ctx->queue->try_push(InputEvent{
            InputControllerKind::MOUSE,
            InputEventKind::RELEASED,
            (unsigned int)Button::RIGHT,
            timestamp,
        });
        break;
      case RI_MOUSE_MIDDLE_BUTTON_DOWN:
        ctx->queue->try_push(InputEvent{
            InputControllerKind::MOUSE,
            InputEventKind::PRESSED,
            (unsigned int)Button::MIDDLE,
            timestamp,
        });
        break;
      case RI_MOUSE_MIDDLE_BUTTON_UP:
        ctx->queue->try_push(InputEvent{
            InputControllerKind::MOUSE,
            InputEventKind::RELEASED,
            (unsigned int)Button::MIDDLE,
            timestamp,
        });
        break;
      case RI_MOUSE_BUTTON_4_DOWN:
        ctx->queue->try_push(InputEvent{
            InputControllerKind::MOUSE,
            InputEventKind::PRESSED,
            (unsigned int)Button::X1,
            timestamp,
        });
        break;
      case RI_MOUSE_BUTTON_4_UP:
        ctx->queue->try_push(InputEvent{
            InputControllerKind::MOUSE,
            InputEventKind::RELEASED,
            (unsigned int)Button::X1,
            timestamp,
        });
        break;
      case RI_MOUSE_BUTTON_5_DOWN:
        ctx->queue->try_push(InputEvent{
            InputControllerKind::MOUSE,
            InputEventKind::PRESSED,
            (unsigned int)Button::X2,
            timestamp,
        });
        break;
      case RI_MOUSE_BUTTON_5_UP:
        ctx->queue->try_push(InputEvent{
            InputControllerKind::MOUSE,
            InputEventKind::RELEASED,
            (unsigned int)Button::X2,
            timestamp,
        });
        break;
      case RI_MOUSE_WHEEL: {
        SHORT wheel = (SHORT)button_data; // gross
        if (wheel > 0) {
          ctx->queue->try_push(InputEvent{
              InputControllerKind::MOUSE,
              InputEventKind::PRESSED,
              (unsigned int)Button::WHEEL_UP,
              timestamp,
          });
        } else {
          ctx->queue->try_push(InputEvent{
              InputControllerKind::MOUSE,
              InputEventKind::PRESSED,
              (unsigned int)Button::WHEEL_DOWN,
              timestamp,
          });
        }
      } break;
      }
    } break;
    }

    return DefWindowProcW(hwnd, msg, wparam, lparam);
  }


} // namespace
namespace aer {


  struct InputWindowsAdapter::Impl {
    HINSTANCE instance;
    WNDCLASSW wc;
    HWND hwnd;
    RAWINPUTDEVICE rid_kb;
    RAWINPUTDEVICE rid_ms;
    std::unique_ptr<CallbackCtx> ctx;
  };


  InputWindowsAdapter::InputWindowsAdapter(std::wstring window_name)
      : pimpl(std::make_unique<Impl>())
      , window_name(window_name) {
    pimpl->instance = GetModuleHandle(nullptr);

    // create window class
    const wchar_t *wc_name = L"AerInput";
    pimpl->wc = {};
    pimpl->wc.lpfnWndProc = wnd_proc;
    pimpl->wc.hInstance = pimpl->instance;
    pimpl->wc.lpszClassName = wc_name;
    if (!RegisterClassW(&pimpl->wc)) {
      throw std::runtime_error(
          "[InputWindowsAdapter] Failed to create AerInput window class");
    }

    // create window
    pimpl->hwnd = CreateWindowW(wc_name, wc_name, 0, 0, 0, 0, 0, HWND_MESSAGE,
                                nullptr, pimpl->instance, nullptr);
    if (pimpl->hwnd == nullptr) {
      throw std::runtime_error(
          "[InputWindowsAdapter] Failed to create AerInput window");
    }

    // create raw input devices
    pimpl->rid_kb = {};
    pimpl->rid_kb.usUsagePage = 0x01;
    pimpl->rid_kb.usUsage = 0x06; // keyboard
    pimpl->rid_kb.dwFlags = RIDEV_EXINPUTSINK;
    pimpl->rid_kb.hwndTarget = pimpl->hwnd;

    pimpl->rid_ms = {};
    pimpl->rid_ms.usUsagePage = 0x01;
    pimpl->rid_ms.usUsage = 0x02; // mouse
    pimpl->rid_ms.dwFlags = RIDEV_EXINPUTSINK;
    pimpl->rid_ms.hwndTarget = pimpl->hwnd;

    if (!RegisterRawInputDevices(&pimpl->rid_kb, 1, sizeof(pimpl->rid_kb))) {
      throw std::runtime_error("[InputWindowAdapter] Failed to register "
                               "keyboard input device");
    }
    if (!RegisterRawInputDevices(&pimpl->rid_ms, 1, sizeof(pimpl->rid_ms))) {
      throw std::runtime_error(
          "[InputWindowAdapter] Failed to register mouse input device");
    }
  }


  InputWindowsAdapter::~InputWindowsAdapter() {
    if (pimpl != nullptr) {
      // unregister rids
      RAWINPUTDEVICE rid[2];
      rid[0].usUsagePage = 0x01;
      rid[0].usUsage = 0x06;
      rid[0].dwFlags = RIDEV_REMOVE;
      rid[0].hwndTarget = pimpl->hwnd;
      rid[1].usUsagePage = 0x01;
      rid[1].usUsage = 0x02;
      rid[1].dwFlags = RIDEV_REMOVE;
      rid[1].hwndTarget = pimpl->hwnd;
      RegisterRawInputDevices(rid, 2, sizeof(RAWINPUTDEVICE));
    }
  }


  void InputWindowsAdapter::poll_input(rigtorp::SPSCQueue<InputEvent> &queue,
                                       const std::chrono::steady_clock &clock,
                                       const std::atomic<double> &epoch) {

    // fetch context
    if (!pimpl->ctx) {
      pimpl->ctx = std::make_unique<CallbackCtx>(
          CallbackCtx{&queue, &clock, &epoch, {}});
      SetWindowLongPtrW(pimpl->hwnd, GWLP_USERDATA, (LONG_PTR)pimpl->ctx.get());
    }

    // process message
    MSG msg{};
    while (PeekMessageW(&msg, pimpl->hwnd, 0, 0, PM_REMOVE)) {
      if (msg.message == WM_QUIT) return;
      if (!is_raylib_window_focused()) {
        continue; // discard event if window is not focused
      }
      TranslateMessage(&msg);
      DispatchMessageW(&msg);
    }
  }


} // namespace aer
#endif