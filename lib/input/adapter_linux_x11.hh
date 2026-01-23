#pragma once

#if defined(__linux__)
#ifdef AER_HAS_LIB_X11
#include "input/input.hh"
#include <rigtorp/SPSCQueue.h>
namespace aer {


  /**
   * @brief Unfortunate workaround. Must be called before raylib's InitWindow if
   * you're planning to use @ref InputHandler with @ref InputX11Adapter.
   *
   * Basically enables multithreading for X11.
   */
  void init_for_input_x11_adapter();


  /**
   * @class InputX11Adapter
   * @brief Input polling device leveraging X11 + XI2. Requires X11; use this
   * with @ref InputHandler
   *
   */
  class InputX11Adapter : public InputPlatformAdapter {
    struct Impl;
    std::unique_ptr<Impl> pimpl;

  private:
    bool x11_is_raylib_window_focused();

  public:
    InputX11Adapter();
    ~InputX11Adapter();

    InputX11Adapter(const InputX11Adapter &) = delete;
    InputX11Adapter &operator=(const InputX11Adapter &other) = delete;

    InputX11Adapter(InputX11Adapter &&) noexcept = default;
    InputX11Adapter &operator=(InputX11Adapter &&) noexcept = default;

    void poll_input(rigtorp::SPSCQueue<InputEvent> &queue,
                    const std::chrono::steady_clock &clock,
                    const std::atomic<double> &epoch);
  };


} // namespace aer
#endif // AER_HAS_LIB_X11
#endif // __linux__
