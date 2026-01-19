#pragma once
#include "rigtorp/SPSCQueue.h"
#include <thread>
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
   * @brief Handles input through a queue. When any input is pressed, an event
   * is sent to the queue. It is the user's job to pop the events, from the main
   * thread.
   *
   * The events are timestamped, with the possibility of resetting the
   * epoch at any point in time. By default, the epoch is the time at which the
   * instance has been constructed.
   *
   * @thread-safe Construct this on the main thread. Input events must never be
   * enqueued from the main thread.
   *
   */
  class InputHandler {
    std::atomic<double> epoch;
    rigtorp::SPSCQueue<InputEvent> queue;
    std::jthread thread;

  private:
    /**
     * @brief Loop over all possible inputs and send events to the queue if
     * input is detected
     *
     * @param queue Input queue
     */
    void poll_input();

    /**
     * @brief Job executed by the input thread
     * @param stop thread stop token
     */
    void run(std::stop_token stop);

  public:
    InputHandler(size_t queue_size);

    /**
     * @brief Get the handler's epoch in milliseconds (relative to the
     * program startup)
     *
     * @return Epoch in ms
     */
    double get_epoch() const { return epoch; }

    /**
     * @brief Get the handler's queue.
     * @warning Do not enqueue anything from the main thread.
     * @return Reference to the input queue
     */
    rigtorp::SPSCQueue<InputEvent> &get_queue() { return queue; }

    /**
     * @brief Set the handler's epoch to the current time.
     * @return New epoch in ms
     */
    double reset_epoch();

    /**
     * @brief Stop the input thread. This instance will become useless.
     */
    void stop();

  private:
  };


} // namespace aer
