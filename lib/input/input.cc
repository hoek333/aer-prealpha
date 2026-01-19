#include "input/input.hh"
#include <raylib.h>
#include <spdlog/spdlog.h>


/* PRIVATE ********************************************************************/


void aer::InputHandler::poll_input() {
  // TODO: urgh... raylib only resets pressed state when PollInputEvents is
  // called, and it MUST be called in the main thread... Seems like we'll have
  // to look for another lib here...
  for (int key = 0; key < 512; key++) {
    if (IsKeyPressed(key)) {
      queue.emplace(InputControllerKind::KEY, InputEventKind::PRESSED, key,
                    GetTime() * 1000 - epoch);
    }
    if (IsKeyReleased(key)) {
      queue.emplace(InputControllerKind::KEY, InputEventKind::RELEASED, key,
                    GetTime() * 1000 - epoch);
    }
  }

  for (int mouse = 0; mouse < 8; mouse++) {
    if (IsMouseButtonPressed(mouse)) {
      queue.emplace(InputControllerKind::MOUSE, InputEventKind::PRESSED, mouse,
                    GetTime() * 1000 - epoch);
    }
    if (IsMouseButtonReleased(mouse)) {
      queue.emplace(InputControllerKind::MOUSE, InputEventKind::RELEASED, mouse,
                    GetTime() * 1000 - epoch);
    }
  }
}


void aer::InputHandler::run(std::stop_token stop) {
  while (!stop.stop_requested()) {
    poll_input();
  }
}


/* PUBLIC *********************************************************************/


aer::InputHandler::InputHandler(size_t queue_size)
    : epoch(GetTime() * 1000)
    , queue(queue_size)
    , thread(&InputHandler::run, this) {}


double aer::InputHandler::reset_epoch() {
  epoch = GetTime() * 1000;
  return epoch;
}


void aer::InputHandler::stop() { thread.request_stop(); }
