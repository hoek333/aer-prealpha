#include "input/factory.hh"
#include "input/input.hh"
#include <memory>
#include <raylib.h>
#include <rigtorp/SPSCQueue.h>
#include <spdlog/spdlog.h>


int main() {
  auto input_handler = aer::create_input_handler(512);

  ChangeDirectory(GetApplicationDirectory());
  SetTraceLogLevel(LOG_WARNING);

  InitWindow(640 * 2, 480 * 2, "aer");
  SetExitKey(KEY_ESCAPE);
  SetConfigFlags(FLAG_WINDOW_RESIZABLE);
  SetWindowState(FLAG_VSYNC_HINT);

  input_handler->start_polling();

  while (!WindowShouldClose()) {
    // placeholder
    aer::InputEvent *ie = input_handler->get_queue().front();
    if (ie != nullptr) {
      spdlog::info("input timestamp: {} ({})", ie->timestamp, ie->code);
      input_handler->get_queue().pop();
    }

    ClearBackground(BLACK);
    BeginDrawing();
    DrawFPS(10, 10);
    EndDrawing();
  }

  input_handler->stop_thread();
  return 0;
}
