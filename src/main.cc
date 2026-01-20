#include "core/platform.hh"
#include "input/fallback.hh"
#include "input/input.hh"
#include "input/linux_x11.hh"
#include <memory>
#include <raylib.h>
#include <rigtorp/SPSCQueue.h>
#include <spdlog/spdlog.h>


int main() {
  aer::Platform platform = aer::detect_platform();

  aer::InputHandler input_handler(512);
  if (platform == aer::Platform::LINUX_X11) {
#ifdef AER_HAS_LIB_X11
    aer::init_for_input_x11_adapter();
    input_handler.set_adapter(std::make_unique<aer::InputX11Adapter>());
#else
    input_handler.set_adapter(std::make_unique<aer::InputFallbackAdapter>());
#endif
  } else {
    input_handler.set_adapter(std::make_unique<aer::InputFallbackAdapter>());
  }
  input_handler.start_polling();

  ChangeDirectory(GetApplicationDirectory());
  SetTraceLogLevel(LOG_WARNING);

  InitWindow(640 * 2, 480 * 2, "aer");
  SetExitKey(KEY_ESCAPE);
  SetConfigFlags(FLAG_WINDOW_RESIZABLE);
  SetWindowState(FLAG_VSYNC_HINT);

  while (!WindowShouldClose()) {
    // placeholder
    aer::InputEvent *ie = input_handler.get_queue().front();
    if (ie != nullptr) {
      spdlog::info("input event: {}", ie->code);
      input_handler.get_queue().pop();
    }

    ClearBackground(BLACK);
    BeginDrawing();
    DrawFPS(10, 10);
    EndDrawing();
  }

  input_handler.stop_thread();
  return 0;
}
