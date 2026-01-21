#if defined(__linux__)
#ifdef AER_HAS_LIB_X11
#include "input/linux_x11.hh"
#include <X11/Xlib.h>
#include <X11/extensions/XInput2.h>
#include <spdlog/spdlog.h>
#include <stdexcept>
namespace aer {


  void init_for_input_x11_adapter() { XInitThreads(); }


  struct InputX11Adapter::Impl {
    Display *display;
    int xi_opcode = -1;
  };


  InputX11Adapter::InputX11Adapter()
      : pimpl(std::make_unique<Impl>()) {
    // create display
    pimpl->display = XOpenDisplay(nullptr);
    if (!pimpl->display) {
      throw std::runtime_error(
          "[InputX11Adapter] Failed to open input X display");
    }
    // query for XInput2
    int event, error;
    if (!XQueryExtension(pimpl->display, "XInputExtension", &pimpl->xi_opcode,
                         &event, &error)) {
      throw std::runtime_error("[InputX11Adapter] XInput2 not available");
    }
    // select event types
    XIEventMask mask;
    unsigned char mask_data[XIMaskLen(XI_LASTEVENT)] = {};
    mask.deviceid = XIAllMasterDevices;
    mask.mask_len = sizeof(mask_data);
    mask.mask = mask_data;
    XISetMask(mask.mask, XI_RawKeyPress);
    XISetMask(mask.mask, XI_RawKeyRelease);
    XISetMask(mask.mask, XI_RawButtonPress);
    XISetMask(mask.mask, XI_RawButtonRelease);
    XISelectEvents(pimpl->display, DefaultRootWindow(pimpl->display), &mask, 1);
    // flush
    XFlush(pimpl->display);
  }


  void InputX11Adapter::poll_input(rigtorp::SPSCQueue<InputEvent> &queue,
                                   const std::chrono::steady_clock &clock,
                                   const std::atomic<double> &epoch) {
    XEvent ev;
    XGenericEventCookie *cookie = &ev.xcookie;
    if (XPending(pimpl->display) == 0) return;

    XNextEvent(pimpl->display, &ev);
    if (!x11_is_raylib_window_focused()) {
      return; // discard event if window is not focused
    }
    auto t = clock.now();
    double timestamp =
        std::chrono::duration<double, std::milli>(t.time_since_epoch())
            .count() -
        epoch;

    if (!XGetEventData(pimpl->display, cookie)) return;
    switch (cookie->evtype) {
    case XI_RawKeyPress:
      queue.try_push(InputEvent{
          InputControllerKind::KEY,
          InputEventKind::PRESSED,
          static_cast<XIRawEvent *>(cookie->data)->detail,
          timestamp,
      });
      break;

    case XI_RawKeyRelease:
      queue.try_push(InputEvent{
          InputControllerKind::KEY,
          InputEventKind::RELEASED,
          static_cast<XIRawEvent *>(cookie->data)->detail,
          timestamp,
      });
      break;

    case XI_RawButtonPress:
      queue.try_push(InputEvent{
          InputControllerKind::MOUSE,
          InputEventKind::PRESSED,
          static_cast<XIRawEvent *>(cookie->data)->detail,
          timestamp,
      });
      break;

    case XI_RawButtonRelease:
      queue.try_push(InputEvent{
          InputControllerKind::MOUSE,
          InputEventKind::RELEASED,
          static_cast<XIRawEvent *>(cookie->data)->detail,
          timestamp,
      });
      break;
    }

    XFreeEventData(pimpl->display, cookie);
  }


  InputX11Adapter::~InputX11Adapter() {
    if (pimpl && pimpl->display) {
      XCloseDisplay(pimpl->display);
    }
  }


} // namespace aer
#endif // AER_HAS_LIB_X11
#endif // __linux__
