#include "input/input.hh"
#include <raylib.h>


aer::InputHandler::InputHandler()
    : epoch(GetTime() * 1000) {}


double aer::InputHandler::update_epoch() {
  epoch = GetTime() * 1000;
  return epoch;
}


void aer::InputHandler::poll_input(
    rigtorp::SPSCQueue<InputEvent> &queue) const {
  for (int key = 0; key < 512; key++) {
    if (IsKeyPressed(key)) {
      queue.try_push(InputEvent{
          InputControllerKind::KEY,
          InputEventKind::PRESSED,
          key,
          GetTime() * 1000 - epoch,
      });
    } else if (IsKeyReleased(key)) {
      queue.try_push(InputEvent{
          InputControllerKind::KEY,
          InputEventKind::RELEASED,
          key,
          GetTime() * 1000 - epoch,
      });
    }
  }

  for (int mouse = 0; mouse < 8; mouse++) {
    if (IsMouseButtonPressed(mouse)) {
      queue.try_push(InputEvent{
          InputControllerKind::MOUSE,
          InputEventKind::RELEASED,
          mouse,
          GetTime() * 1000 - epoch,
      });
    } else if (IsMouseButtonPressed(mouse)) {
      queue.try_push(InputEvent{
          InputControllerKind::MOUSE,
          InputEventKind::RELEASED,
          mouse,
          GetTime() * 1000 - epoch,
      });
    }
  }
}
