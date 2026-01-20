#include "input/fallback.hh"
#include <chrono>
#include <thread>


aer::InputDefaultAdapter::InputDefaultAdapter() {}


void aer::InputDefaultAdapter::poll_input(
    [[maybe_unused]] rigtorp::SPSCQueue<InputEvent> &queue,
    [[maybe_unused]] const std::chrono::steady_clock &clock) {
  std::this_thread::sleep_for(std::chrono::milliseconds(1));
}
