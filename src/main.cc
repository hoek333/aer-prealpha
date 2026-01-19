#include "input_thread.hh"
#include <raylib.h>
#include <thread>


int main() {
  ChangeDirectory(GetApplicationDirectory());
  SetTraceLogLevel(LOG_WARNING);

  InitWindow(640 * 2, 480 * 2, "aer");
  SetExitKey(KEY_ESCAPE);
  SetConfigFlags(FLAG_WINDOW_RESIZABLE);
  SetWindowState(FLAG_VSYNC_HINT);

  std::jthread input_thread(aer::input_job);

  while (!WindowShouldClose()) {
    ClearBackground(BLACK);
    BeginDrawing();
    DrawFPS(10, 10);
    EndDrawing();
  }

  input_thread.request_stop();
  return 0;
}
