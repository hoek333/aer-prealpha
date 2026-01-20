#pragma once
#include <atomic>
#include <chrono>
#include <concepts>
#include <ratio>
#include <rigtorp/SPSCQueue.h>
#include <thread>
#include <type_traits>
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


  template <class T>
  concept IsInputPlatformAdapter =
      requires(T t, rigtorp::SPSCQueue<InputEvent> &queue,
               const std::chrono::steady_clock &clock) {
        { t.poll_input(queue, clock) } -> std::same_as<void>;
      } //
      && std::is_default_constructible_v<T>;


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
   * @thread-safe. Construct this on the main thread. Input events must never be
   * enqueued from the main thread.
   *
   * @tparam InputPlatformAdapter OS-specific input polling device
   */
  template <class InputPlatformAdapter>
    requires IsInputPlatformAdapter<InputPlatformAdapter>
  class InputHandler {
    std::chrono::steady_clock clock; // inner clock
    std::atomic<double> epoch;       // zero-time that timestamps reference (ms)
    rigtorp::SPSCQueue<InputEvent> queue; // InputEvent queue
    InputPlatformAdapter adapter;         // OS adapter
    std::jthread thread;                  // poll thread

  private:
    /**
     * @brief Job executed by the input thread
     * @param stop thread stop token
     */
    void run(std::stop_token stop) {
      while (!stop.stop_requested()) {
        adapter.poll_input(queue, clock);
      }
    }

  public:
    InputHandler(size_t queue_size)
        : clock()
        , epoch()
        , queue(queue_size)
        , adapter()
        , thread() {
      reset_epoch();
      thread = std::jthread(&InputHandler::run, this);
    }

    /**
     * @brief Get the handler's queue.
     * @warning Do not enqueue anything from the main thread. You can however
     * pop events as you want.
     * @return Reference to the input queue
     */
    rigtorp::SPSCQueue<InputEvent> &get_queue() { return queue; }

    /**
     * @brief Get the underlying platform device.
     * @return Reference to the platform adapter device.
     */
    const InputPlatformAdapter &get_adapter() const { return adapter; }

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
    double reset_epoch() {
      auto t = clock.now();
      epoch = std::chrono::duration<double, std::milli>(t.time_since_epoch())
                  .count();
      return epoch;
    }

    /**
     * @brief Stop the input thread. This instance will become useless.
     */
    void stop() { thread.request_stop(); }
  };


} // namespace aer
