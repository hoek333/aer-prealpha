#include "input/factory.hh"
#include "core/platform.hh"
#include "input/fallback.hh"
#include "input/input.hh"
#include "input/linux_x11.hh"
#include "input/windows.hh"
#include <memory>


std::unique_ptr<aer::InputHandler>
aer::create_input_handler(size_t queue_size) {
  aer::Platform platform = aer::detect_platform();
  auto ret = std::make_unique<InputHandler>(queue_size);
  switch (platform) {

  case Platform::LINUX_X11:
#if defined(__linux__)
#ifdef AER_HAS_LIB_X11
    aer::init_for_input_x11_adapter();
    ret->set_adapter(std::make_unique<InputX11Adapter>());
#else
    ret->set_adapter(std::make_unique<aer::InputFallbackAdapter>());
#endif
#else
    ret->set_adapter(std::make_unique<aer::InputFallbackAdapter>());
#endif
    break;

  case Platform::WINDOWS:
#if defined(_WIN32)
    ret->set_adapter(std::make_unique<aer::InputWindowsAdapter>(L"aer"));
#else
    ret->set_adapter(std::make_unique<aer::InputFallbackAdapter>());
#endif
    break;

  default:
    ret->set_adapter(std::make_unique<InputFallbackAdapter>());
    break;
  }

  return ret;
}
