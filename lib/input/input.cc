#include "input/input.hh"


void aer::InputHandler::run(std::stop_token stop) {
  while (!stop.stop_requested()) {
    if (adapter != nullptr && polling) {
      adapter->poll_input(queue, clock);
    }
  }
}


aer::InputHandler::InputHandler(size_t queue_size)
    : clock()
    , epoch()
    , queue(queue_size)
    , adapter()
    , thread() {
  reset_epoch();
  thread = std::jthread(&InputHandler::run, this);
}


double aer::InputHandler::reset_epoch() {
  auto t = clock.now();
  epoch =
      std::chrono::duration<double, std::milli>(t.time_since_epoch()).count();
  return epoch;
}
