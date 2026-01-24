#pragma once
#include <chrono>
namespace aer {


  /**
   * @brief Check if the raylib window is currently focused
   * @return true if focused
   */
  bool is_raylib_window_focused();


  /**
   * @brief Get current timestamp relative to provided epoch
   * @param clock
   * @param epoch
   * @return Current timestamp
   */
  double get_now(const std::chrono::steady_clock &clock, double epoch);


} // namespace aer
