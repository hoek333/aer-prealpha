#include "input/input.hh"
#include "input/linux_x11.hh"
#include <raylib.h>
#include <rigtorp/SPSCQueue.h>
#include <spdlog/spdlog.h>


int main() {
#ifdef AER_HAS_X11
  aer::init_for_input_x11_adapter();
#endif // AER_HAS_X11

  ChangeDirectory(GetApplicationDirectory());
  SetTraceLogLevel(LOG_WARNING);

  InitWindow(640 * 2, 480 * 2, "aer");
  SetExitKey(KEY_ESCAPE);
  SetConfigFlags(FLAG_WINDOW_RESIZABLE);
  SetWindowState(FLAG_VSYNC_HINT);

#ifdef AER_HAS_X11
  aer::InputHandler<aer::InputX11Adapter> input_handler(520);
#else
  aer::InputHandler<aer::InputDefaultAdapter> input_handler(520);
#endif

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
