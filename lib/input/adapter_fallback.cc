#include "input/adapter_fallback.hh"
#include <chrono>
#include <thread>


aer::InputFallbackAdapter::InputFallbackAdapter() {}


void aer::InputFallbackAdapter::poll_input(
    [[maybe_unused]] rigtorp::SPSCQueue<InputEvent> &queue,
    [[maybe_unused]] const std::atomic<double> &epoch) {
  std::this_thread::sleep_for(std::chrono::milliseconds(1));
}
