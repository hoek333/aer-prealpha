#include "input/input.hh"
#include <raylib.h>


aer::InputHandler::InputHandler()
    : epoch(GetTime() * 1000) {}


double aer::InputHandler::update_epoch() {
  epoch = GetTime() * 1000;
  return epoch;
}


void aer::InputHandler::poll_input(std::vector<InputEvent> queue) const {
  for (int key = 0; key < 512; key++) {
    if (IsKeyPressed(key)) {
      queue.push_back(InputEvent{
          InputControllerKind::KEY,
          InputEventKind::PRESSED,
          key,
          GetTime() * 1000 - epoch,
      });
    } else if (IsKeyReleased(key)) {
      queue.push_back(InputEvent{
          InputControllerKind::KEY,
          InputEventKind::RELEASED,
          key,
          GetTime() * 1000 - epoch,
      });
    }
  }

  for (int mouse = 0; mouse < 8; mouse++) {
    if (IsMouseButtonPressed(mouse)) {
      queue.push_back(InputEvent{
          InputControllerKind::MOUSE,
          InputEventKind::PRESSED,
          mouse,
          GetTime() * 1000 - epoch,
      });
    } else if (IsMouseButtonPressed(mouse)) {
      queue.push_back(InputEvent{
          InputControllerKind::MOUSE,
          InputEventKind::RELEASED,
          mouse,
          GetTime() * 1000 - epoch,
      });
    }
  }
}
