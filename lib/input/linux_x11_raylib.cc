#include "input/linux_x11.hh"
#include <raylib.h>


bool aer::InputX11Adapter::x11_is_raylib_window_focused() {
  // we have to define this function in a separate file to avoid naming
  // conflicts between X11 and Raylib
  return IsWindowFocused();
}
