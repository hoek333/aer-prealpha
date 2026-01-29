#if defined(_WIN32)
#include "input/adapter_windows.hh"
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
    const std::atomic<double> *epoch;
    bool kb_state[256] =
        {}; // keyboard state
            // we have to keep track of whether or not the keys are pressed to
            // prevent queuing auto-repeat events because WM_INPUT does not tell
            // you if a press event is caused by auto-repeat. because the API
            // sucks
  };


  LRESULT CALLBACK wnd_proc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
    // discard event if window is not focused
    if (!is_raylib_window_focused()) {
      return DefWindowProcW(hwnd, msg, wparam, lparam);
    }

    // get context
    auto *ctx = (CallbackCtx *)GetWindowLongPtrW(hwnd, GWLP_USERDATA);
    if (!ctx || ctx->queue == nullptr || ctx->clock == nullptr ||
        ctx->epoch == nullptr) {
      return DefWindowProcW(hwnd, msg, wparam, lparam);
    }

    // timestamp
    double timestamp = get_now(*ctx->epoch);

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
      // get extended scancode
      USHORT scancode = raw.data.keyboard.MakeCode;
      if (raw.data.keyboard.Flags & RI_KEY_E0) scancode |= 0xE000;
      if (raw.data.keyboard.Flags & RI_KEY_E1) {
        // discard pause button
        return DefWindowProcW(hwnd, msg, wparam, lparam);
      }
      // queue
      if (!(raw.data.keyboard.Flags & RI_KEY_BREAK)) { // key press
        if (!ctx->kb_state[scancode]) {
          ctx->queue->try_push(InputEvent{
              InputControllerKind::KEY,
              InputEventKind::PRESSED,
              (uint8_t)translate_windows_vscex(scancode),
              timestamp,
          });
          ctx->kb_state[scancode] = true;
        }
      } else { // key release
        ctx->queue->try_push(InputEvent{
            InputControllerKind::KEY,
            InputEventKind::RELEASED,
            (uint8_t)translate_windows_vscex(scancode),
            timestamp,
        });
        ctx->kb_state[scancode] = false;
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

  Key translate_windows_vscex(USHORT vscex) {
    // mapping based on https://kbdlayout.info &
    // https://learn.microsoft.com/en-us/windows/win32/inputdev/about-keyboard-input
    switch (vscex) {
      // clang-format off
    case 0x01: return Key::ESC; break;
    case 0x3B: return Key::F1; break;
    case 0x3C: return Key::F2; break;
    case 0x3D: return Key::F3; break;
    case 0x3E: return Key::F4; break;
    case 0x3F: return Key::F5; break;
    case 0x40: return Key::F6; break;
    case 0x41: return Key::F7; break;
    case 0x42: return Key::F8; break;
    case 0x43: return Key::F9; break;
    case 0x44: return Key::F10; break;
    case 0x57: return Key::F11; break;
    case 0x58: return Key::F12; break;

    case 0x68: return Key::F13; break;
    case 0x69: return Key::F14; break;
    case 0x6A: return Key::F15; break;
    case 0x6B: return Key::F16; break;
    case 0x6C: return Key::F17; break;
    case 0x6D: return Key::F18; break;
    case 0x6E: return Key::F19; break;
    case 0x6F: return Key::F20; break;
    case 0x70: return Key::F21; break; // aka HIRAGANA_KATAKANA
    case 0x71: return Key::F22; break;
    case 0x72: return Key::F23; break;
    case 0x73: return Key::F24; break; // aka BACKSLASH_UNDERSCORES

    case 0x54: return Key::PRINT_SCREEN; break;
    case 0x46: return Key::SCROLL_LOCK; break;
    case 0xE1ED: return Key::PAUSE; break;

    case 0x29: return Key::GRAVE; break; // aka HANKAKU_ZENKAKU
    case 0x02: return Key::ONE; break;
    case 0x03: return Key::TWO; break;
    case 0x04: return Key::THREE; break;
    case 0x05: return Key::FOUR; break;
    case 0x06: return Key::FIVE; break;
    case 0x07: return Key::SIX; break;
    case 0x08: return Key::SEVEN; break;
    case 0x09: return Key::EIGHT; break;
    case 0x0A: return Key::NINE; break;
    case 0x0B: return Key::ZERO; break;
    case 0x0C: return Key::MINUS; break;
    case 0x0D: return Key::EQUAL; break;
    case 0x0E: return Key::BACKSPACE; break;

    case 0x0F: return Key::TAB; break;
    case 0x10: return Key::Q; break;
    case 0x11: return Key::W; break;
    case 0x12: return Key::E; break;
    case 0x13: return Key::R; break;
    case 0x14: return Key::T; break;
    case 0x15: return Key::Y; break;
    case 0x16: return Key::U; break;
    case 0x17: return Key::I; break;
    case 0x18: return Key::O; break;
    case 0x19: return Key::P; break;
    case 0x1A: return Key::BRACKET_LEFT; break;
    case 0x1B: return Key::BRACKET_RIGHT; break;
    case 0x1C: return Key::RETURN; break;

    case 0x3A: return Key::CAPS; break;
    case 0x1E: return Key::A; break;
    case 0x1F: return Key::S; break;
    case 0x20: return Key::D; break;
    case 0x21: return Key::F; break;
    case 0x22: return Key::G; break;
    case 0x23: return Key::H; break;
    case 0x24: return Key::J; break;
    case 0x25: return Key::K; break;
    case 0x26: return Key::L; break;
    case 0x27: return Key::SEMICOLON; break;
    case 0x28: return Key::APOSTROPHE; break;
    case 0x2B: return Key::BACKSLASH; break;

    case 0x2A: return Key::LEFT_SHIFT; break;
    case 0x56: return Key::BACKSLASH; break;
    case 0x2C: return Key::Z; break;
    case 0x2D: return Key::X; break;
    case 0x2E: return Key::C; break;
    case 0x2F: return Key::V; break;
    case 0x30: return Key::B; break;
    case 0x31: return Key::N; break;
    case 0x32: return Key::M; break;
    case 0x33: return Key::COMMA; break;
    case 0x34: return Key::PERIOD; break;
    case 0x35: return Key::SLASH; break;
    case 0x36: return Key::RIGHT_SHIFT; break;

    case 0x1D: return Key::LEFT_CONTROL; break;
    case 0xE05B: return Key::LEFT_SUPER; break;
    case 0x38: return Key::LEFT_ALT; break;
    case 0x39: return Key::SPACE; break;
    case 0xE038: return Key::RIGHT_ALT; break;
    case 0xE05C: return Key::RIGHT_SUPER; break;
    case 0xE05D: return Key::MENU; break;
    case 0xE01D: return Key::RIGHT_CONTROL; break;

    case 0xE052: return Key::INSERT; break;
    case 0xE047: return Key::HOME; break;
    case 0xE049: return Key::PAGE_UP; break;
    case 0xE053: return Key::DELETE_; break;
    case 0xE04F: return Key::END; break;
    case 0xE051: return Key::PAGE_DOWN; break;

    case 0xE04B: return Key::LEFT; break;
    case 0xE048: return Key::UP; break;
    case 0xE050: return Key::DOWN; break;
    case 0xE04D: return Key::RIGHT; break;

    case 0x45: return Key::PAUSE; break;
    case 0xE035: return Key::NUM_DIVIDE; break;
    case 0x37: return Key::NUM_MULTIPLY; break;
    case 0x4A: return Key::NUM_SUBTRACT; break;
    case 0x4E: return Key::NUM_ADD; break;
    case 0xE01C: return Key::NUM_ENTER; break;
    case 0x47: return Key::NUM_SEVEN; break;
    case 0x48: return Key::NUM_EIGHT; break;
    case 0x49: return Key::NUM_NINE; break;
    case 0x4B: return Key::NUM_FOUR; break;
    case 0x4C: return Key::NUM_FIVE; break;
    case 0x4D: return Key::NUM_SIX; break;
    case 0x4F: return Key::NUM_ONE; break;
    case 0x50: return Key::NUM_TWO; break;
    case 0x51: return Key::NUM_THREE; break;
    case 0x52: return Key::NUM_ZERO; break;
    case 0x53: return Key::NUM_DELETE; break;

    // case 0x29: return Key::HANKAKU_ZENKAKU; break; // duplicate
    case 0x7B: return Key::MUHENKAN; break; // aka HANGUL_LATIN
    case 0x79: return Key::HENKAN; break; // aka HANGUL_HANJA
    // case 0x70: return Key::HIRAGANA_KATAKANA; break; // duplicate
    // case 0x73: return Key::BACKSLASH_UNDERSCORE; break; // duplicate
    case 0x7D: return Key::YEN; break;
    case 0x7E: return Key::KP_JP_COMMA; break; // unsure

    // case 0x7B: return Key::HANGUL_LATIN; break; // duplicate
    // case 0x79: return Key::HANGUL_HANJA; break; // duplicate
      // clang-format on

    default:
      return Key::NONE;
      break;
    }
  }


  struct InputWindowsAdapter::Impl {
    HINSTANCE instance;
    WNDCLASSW wc;
    const wchar_t *wc_name;
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
    pimpl->wc_name = L"AerInput";
    pimpl->wc = {};
    pimpl->wc.lpfnWndProc = wnd_proc;
    pimpl->wc.hInstance = pimpl->instance;
    pimpl->wc.lpszClassName = pimpl->wc_name;
    if (!RegisterClassW(&pimpl->wc)) {
      throw std::runtime_error(
          "[InputWindowsAdapter] Failed to create AerInput window class");
    }

    // create window
    pimpl->hwnd =
        CreateWindowW(pimpl->wc_name, pimpl->wc_name, 0, 0, 0, 0, 0,
                      HWND_MESSAGE, nullptr, pimpl->instance, nullptr);
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
      // destroy window
      DestroyWindow(pimpl->hwnd);
      // unregister window class
      UnregisterClassW(pimpl->wc_name, pimpl->instance);
    }
  }


  void InputWindowsAdapter::poll_input(rigtorp::SPSCQueue<InputEvent> &queue,
                                       const std::atomic<double> &epoch) {

    // fetch context
    if (!pimpl->ctx) {
      pimpl->ctx =
          std::make_unique<CallbackCtx>(CallbackCtx{&queue, &epoch, {}});
      SetWindowLongPtrW(pimpl->hwnd, GWLP_USERDATA, (LONG_PTR)pimpl->ctx.get());
    }

    // process message
    // NOTE: RAWINPUT messages are directly sent to wnd_proc instead of the
    // listener window's message queue.
    MSG msg{};
    while (PeekMessageW(&msg, pimpl->hwnd, 0, 0, PM_REMOVE)) {
      if (msg.message == WM_QUIT) return;
      TranslateMessage(&msg);
      DispatchMessageW(&msg);
    }
  }


} // namespace aer
#endif
