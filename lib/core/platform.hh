#pragma once
namespace aer {


  enum struct Platform {
    UNKNOWN = 0,
    LINUX_X11,
    WINDOWS,
  };


  bool is_x11_available();
  bool is_wayland_available();
  Platform detect_platform();

} // namespace aer
