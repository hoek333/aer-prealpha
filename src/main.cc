#include "input/factory.hh"
#include "input/input.hh"
#include "input/key.hh"
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
    auto input_events = input_handler->consume_events();
    for (const auto &ie : input_events) {
      bool kind;
      if (ie.kind == aer::InputEventKind::PRESSED) {
        kind = true;
      } else {
        kind = false;
      }
      spdlog::info("input timestamp: {} ({}/{})", ie.timestamp, ie.code, kind);
    }

    ClearBackground(BLACK);
    BeginDrawing();
    DrawFPS(10, 10);
    EndDrawing();
  }

  input_handler->stop_thread();
  return 0;
}
