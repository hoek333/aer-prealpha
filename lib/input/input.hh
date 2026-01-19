#pragma once
#include <vector>
namespace aer {


  enum struct InputControllerKind {
    KEY,
    MOUSE,
    // GAMEPAD,
  };


  enum struct InputEventKind {
    PRESSED,
    RELEASED,
  };


  /**
   * @class InputEvent
   * @brief Represents a single input event.
   *
   */
  struct InputEvent {
    InputControllerKind controller;
    InputEventKind kind;
    int code;
    double timestamp; // in milliseconds
  };


  /**
   * @class InputHandler
   * @brief Input handler. Provides a way to send relative timestamped @ref
   * InputEvent to a queue
   *
   */
  class InputHandler {
    double epoch;

  public:
    InputHandler();

    /**
     * @brief Get the handler's epoch time in milliseconds (relative to the
     * program startup)
     *
     * @return Epoch time in ms
     */
    double get_epoch() const { return epoch; }

    /**
     * @brief Set the handler's epoch time to now
     * @return New epoch time in ms
     */
    double update_epoch();

    /**
     * @brief Loop over all possible inputs and send events to the queue if
     * input is detected
     *
     * @param queue Input queue. TODO: type is placeholder
     */
    void poll_input(std::vector<InputEvent> queue) const;
  };


} // namespace aer
