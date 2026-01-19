#include "input/input.hh"
#include <raylib.h>
#include <rigtorp/SPSCQueue.h>
#include <spdlog/spdlog.h>


int main() {
  ChangeDirectory(GetApplicationDirectory());
  SetTraceLogLevel(LOG_WARNING);

  InitWindow(640 * 2, 480 * 2, "aer");
  SetExitKey(KEY_ESCAPE);
  SetConfigFlags(FLAG_WINDOW_RESIZABLE);
  SetWindowState(FLAG_VSYNC_HINT);

  aer::InputHandler input_handler(520);

  while (!WindowShouldClose()) {
    ClearBackground(BLACK);
    BeginDrawing();
    DrawFPS(10, 10);
    EndDrawing();
    spdlog::info("{}", input_handler.get_queue().size());
  }

  input_handler.stop();
  return 0;
}
