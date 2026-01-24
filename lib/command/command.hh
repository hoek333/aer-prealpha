#pragma once
#include "input/key.hh"
#include <any>
#include <memory>
#include <unordered_map>
namespace aer {


  /**
   * @class Command
   * @brief Command abstract class
   *
   */
  struct Command {
    virtual ~Command() = default;
    /**
     * @brief Execute function. It is recommended for you to define
     *
     * @param args
     */
    virtual void execute(std::any args) const = 0;
  };


  /**
   * @brief Unwrap the arguments of a command.
   * @throws std::runtime_error if unwrapping fails.
   * @tparam T Args class to unwrap
   * @param a Args passed as std::any
   * @return Unwrapped args
   */
  template <class T> T unwrap_command_args(const std::any &a) {
    try {
      return std::any_cast<T>(a);
    } catch (const std::bad_any_cast &e) {
      throw std::runtime_error(
          "A Command was executed with unexpected args! {}");
    }
  }


  /**
   * @class CommandRegistry
   * @brief Registry of commands. Each @ref Command you add is assigned to an
   * ID. You can freely bind or unbind a @ref Key to any @ref Command.
   * Internally, the bindings are stored as two maps: a (Key -> ID) map and a
   * (ID -> Command) map. You can find a @ref Command instance in the registry
   * either by @ref Key or by ID.
   *
   */
  class CommandRegistry {
    std::unordered_map<Key, unsigned int> key_map;
    std::unordered_map<unsigned int, std::unique_ptr<Command>> cmd_map;

  public:
    CommandRegistry() = default;

    /**
     * @brief Add a new command to the pool. No key will be bound
     * @param command
     * @param id ID to map the command to
     * @return true if successful, false if the ID is already taken
     */
    bool add_command(unsigned int id, std::unique_ptr<Command> command);

    /**
     * @brief Bind a command to a key
     * @param key Key value
     * @param id ID of the command to bind
     * @return true if successful, false if the key is already bound
     */
    bool bind_key(Key key, unsigned int id);

    /**
     * @brief Unbind a command from its key
     * @param id ID of the command to unbind
     * @return true if successful
     */
    bool unbind_key(unsigned int id);

    /**
     * @brief Find a command by keybind
     * @param key Key to which the command is bound
     */
    const Command *find_command(Key key) const;

    /**
     * @brief Find a command by ID
     * @param id ID of the command
     */
    const Command *find_command(unsigned int id) const;
  };


} // namespace aer
