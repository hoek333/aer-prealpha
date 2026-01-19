#include "input_thread.hh"
#include <raylib.h>
#include <spdlog/spdlog.h>
#include <stop_token>


void aer::input_job(std::stop_token stop) {
  while (!stop.stop_requested()) {

    // TODO: placeholder
    if (IsKeyDown(KEY_A)) {
      spdlog::info("a is down");
    }
  }
}
