#pragma once
#include <atomic>
#include <chrono>
#include <rigtorp/SPSCQueue.h>
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
    uint8_t code;     // corresponds to a Key enum if keyboard event; Mouse
                      // enum if mouse event
    double timestamp; // in milliseconds
  };


  class InputPlatformAdapter {
  public:
    virtual void poll_input(rigtorp::SPSCQueue<InputEvent> &queue,
                            const std::atomic<double> &epoch) = 0;
  };


  /**
   * @class InputHandler
   * @brief Handles input through a queue. When any input is pressed, an event
   * is sent to the queue. It is the user's job to free up the queue by popping
   * events and processing them.
   *
   * The events are timestamped with the possibility of resetting the epoch
   * (zero-time) at any point in time. By default, the epoch is the time at
   * which the instance has been constructed.
   *
   * @thread-safe Construct this on the main thread. Input events must never be
   * enqueued from the main thread.
   */
  class InputHandler {
    std::atomic<double> epoch; // zero-time that timestamps reference (ms)
    rigtorp::SPSCQueue<InputEvent> queue;          // InputEvent queue
    std::unique_ptr<InputPlatformAdapter> adapter; // OS adapter
    std::jthread thread;                           // input thread
    std::atomic<bool> polling = false; // true: poll input. false: do not

  private:
    /**
     * @brief Job executed by the input thread
     * @param stop thread stop token
     */
    void run(std::stop_token stop);

  public:
    InputHandler(size_t queue_size);

    /**
     * @brief Get the handler's queue.
     * @warning Do not enqueue anything from the main thread. You can however
     * pop events as you want.
     * @return Reference to the input queue
     */
    rigtorp::SPSCQueue<InputEvent> &get_queue() { return queue; }

    /**
     * @brief Get the handler's epoch in milliseconds (relative to the
     * program startup)
     *
     * @return Epoch in ms
     */
    double get_epoch() const { return epoch; }

    /**
     * @brief Set the handler's epoch to the current time.
     * @return New epoch in ms
     */
    double reset_epoch();

    /**
     * @brief Set the handler's adapter. You must do this before you begin
     * polling, otherwise the thing will not do anything.
     * @param adapter Adapter that the handler should use
     *
     * @thread-safe This is not thread-safe. Do not change the adapter while
     * polling.
     */
    void set_adapter(std::unique_ptr<InputPlatformAdapter> adapter) {
      this->adapter = std::move(adapter);
    }

    /**
     * @brief Return the polling status of the handler
     * @return true if currently polling input, false if not
     */
    bool is_polling() const { return polling; }

    /**
     * @brief It is best to start polling after the raylib window has been
     * created. For the time being you can start polling before the window
     * creation, but we have no guarantee that this behavior will stick in
     * future Raylib versions.
     */
    void start_polling() { polling = true; }
    void pause_polling() { polling = false; }

    /**
     * @brief Consume all events older than the current time. You will typically
     * call this function at the beginning of every frame.
     * @return A vector containing all the consumed events. They are sorted from
     * oldest to newest.
     */
    std::vector<InputEvent> consume_events();

    /**
     * @brief Stop the input thread. This instance will become useless.
     */
    void stop_thread() { thread.request_stop(); }
  };


} // namespace aer
