#pragma once
#include <stop_token>
namespace aer {


  /**
   * @brief Job executed by the input thread
   * @param stop thread stop token
   */
  void input_job(std::stop_token stop);


} // namespace aer
