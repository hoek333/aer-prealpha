#pragma once
#ifdef AER_HAS_X11
#include "input/input.hh"
#include <rigtorp/SPSCQueue.h>
namespace aer {


  /**
   * @brief Unfortunate workaround. Must be called before raylib's InitWindow
   */
  void init_for_input_x11_adapter();


  class InputX11Adapter {
    struct Impl;
    std::unique_ptr<Impl> pimpl;

  public:
    InputX11Adapter();
    ~InputX11Adapter();

    void poll_input(rigtorp::SPSCQueue<InputEvent> &queue,
                    const std::chrono::steady_clock &clock);
  };


} // namespace aer
#endif // AER_HAS_X11
