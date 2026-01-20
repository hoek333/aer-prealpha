#pragma once
#include "input/input.hh"
#include <rigtorp/SPSCQueue.h>
namespace aer {


  class InputDefaultAdapter {
  public:
    InputDefaultAdapter();
    void poll_input([[maybe_unused]] rigtorp::SPSCQueue<InputEvent> &queue,
                    [[maybe_unused]] const std::chrono::steady_clock &clock);
  };


} // namespace aer
