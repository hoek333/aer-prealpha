#include "input/linux_x11.hh"
#include <raylib.h>


// we have to define this function in a separate file to avoid conflicts
// between X11 and Raylib because none of them prefixed their names :(
bool aer::InputX11Adapter::x11_is_raylib_window_focused() {
  return IsWindowFocused();
}
