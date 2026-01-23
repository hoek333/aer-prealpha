#pragma once

#if defined(_WIN32)
#include "input/input.hh"
namespace aer {


  class InputWindowsAdapter : public InputPlatformAdapter {
    struct Impl;
    std::unique_ptr<Impl> pimpl;
    std::wstring window_name;

  public:
    InputWindowsAdapter(std::wstring window_name);
    ~InputWindowsAdapter();

    void poll_input(rigtorp::SPSCQueue<InputEvent> &queue,
                    const std::chrono::steady_clock &clock,
                    const std::atomic<double> &epoch);
  };


} // namespace aer
#endif
