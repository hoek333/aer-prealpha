#pragma once
#include "input/_fwd.hh"
#include <cstddef>
#include <memory>
namespace aer {


  /**
   * @brief Automatically detect platform and create an input handler with
   * the corresponding adapter.
   *
   * @param queue_size Size of the input queue
   */
  std::unique_ptr<InputHandler> create_input_handler(size_t queue_size);


} // namespace aer
