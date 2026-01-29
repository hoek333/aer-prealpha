#pragma once
#include "input/input.hh"
#include <rigtorp/SPSCQueue.h>
namespace aer {


  class InputFallbackAdapter : public InputPlatformAdapter {
  public:
    InputFallbackAdapter();
    void poll_input([[maybe_unused]] rigtorp::SPSCQueue<InputEvent> &queue,
                    [[maybe_unused]] const std::atomic<double> &epoch);
  };


} // namespace aer
