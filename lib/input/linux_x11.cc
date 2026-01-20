#ifdef AER_HAS_X11
#include "input/linux_x11.hh"
#include <X11/Xlib.h>
#include <X11/extensions/XInput2.h>
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
    XISetMask(mask.mask, XI_ButtonPress);
    XISetMask(mask.mask, XI_ButtonRelease);
    XISelectEvents(pimpl->display, DefaultRootWindow(pimpl->display), &mask, 1);
    // flush
    XFlush(pimpl->display);
  }


  void InputX11Adapter::poll_input(rigtorp::SPSCQueue<InputEvent> &queue,
                                   const std::chrono::steady_clock &clock) {
    XEvent ev;
    XGenericEventCookie *cookie = &ev.xcookie;
    if (XPending(pimpl->display) == 0) return;

    XNextEvent(pimpl->display, &ev);
    auto t = clock.now();
    double ts =
        std::chrono::duration<double, std::milli>(t.time_since_epoch()).count();

    if (!XGetEventData(pimpl->display, cookie)) return;
    if (cookie->evtype == XI_RawKeyPress) {
      queue.try_push(InputEvent{
          InputControllerKind::KEY,
          InputEventKind::PRESSED,
          1, // TODO: provide the key code but for real
          ts,
      });
    }
    if (cookie->evtype == XI_RawKeyRelease) {
      queue.try_push(InputEvent{
          InputControllerKind::KEY,
          InputEventKind::RELEASED,
          1,
          ts,
      });
    }

    XFreeEventData(pimpl->display, cookie);
  }


  InputX11Adapter::~InputX11Adapter() {
    if (pimpl->display) {
      XCloseDisplay(pimpl->display);
    }
  }


} // namespace aer
#endif // AER_HAS_X11
