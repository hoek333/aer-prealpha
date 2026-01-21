#include "core/platform.hh"
#include <cstdlib>


bool aer::is_x11_available() {
#ifdef AER_HAS_LIB_X11
  return std::getenv("DISPLAY") != nullptr;
#else
  return false;
#endif
}


bool aer::is_wayland_available() {
  return std::getenv("WAYLAND_DISPLAY") != nullptr;
}


aer::Platform aer::detect_platform() {
#if defined(__linux__)
  if (is_x11_available()) {
    return Platform::LINUX_X11;
  } else {
    return Platform::UNKNOWN;
  }
#endif
#if defined(_WIN32)
  return Platform::WINDOWS;
#endif
}
