#include "input/input.hh"
#include "input/_utils.hh"
#include <raylib.h>


void aer::InputHandler::run(std::stop_token stop) {
  while (!stop.stop_requested()) {
    if (adapter != nullptr && polling) {
      adapter->poll_input(queue, epoch);
    }
  }
}


aer::InputHandler::InputHandler(size_t queue_size)
    : epoch()
    , queue(queue_size)
    , adapter()
    , thread() {
  reset_epoch();
  thread = std::jthread(&InputHandler::run, this);
}


double aer::InputHandler::reset_epoch() { return get_now(0); }


std::vector<aer::InputEvent> aer::InputHandler::consume_events() {
  double now = get_now(epoch);
  std::vector<aer::InputEvent> ret;
  if (queue.empty()) return ret;
  ret.reserve(32);

  // consume all events prior to current time
  while (true) {
    auto *ie = queue.front();
    if (ie == nullptr || ie->timestamp > now) break;
    ret.push_back(*ie);
    queue.pop();
  }

  return ret;
}
