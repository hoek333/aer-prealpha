#include "input/_utils.hh"
#include <chrono>
#include <raylib.h>


// we have to define this function in a separate file to avoid naming conflicts
// between system libs and raylib
bool aer::is_raylib_window_focused() { return IsWindowFocused(); }


double aer::get_now(double epoch) {
  auto t = std::chrono::steady_clock::now();
  return std::chrono::duration<double, std::milli>(t.time_since_epoch())
             .count() -
         epoch;
}
