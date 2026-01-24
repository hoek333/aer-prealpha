#include "command/command.hh"


bool aer::CommandRegistry::add_command(unsigned int id,
                                       std::unique_ptr<Command> command) {
  return cmd_map.try_emplace(id, std::move(command)).second;
}


bool aer::CommandRegistry::bind_key(Key key, unsigned int id) {
  if (!cmd_map.contains(id)) return false;
  return key_map.try_emplace(key, id).second;
}


bool aer::CommandRegistry::unbind_key(unsigned int id) {
  if (!cmd_map.contains(id)) return false;
  for (auto it = key_map.begin(); it != key_map.end(); ++it) {
    if (it->second == id) {
      key_map.erase(it->first);
      return true;
    }
  }
  return false;
}


const aer::Command *aer::CommandRegistry::find_command(Key key) const {
  if (!key_map.contains(key)) return nullptr;
  unsigned int id = key_map.at(key);
  if (!cmd_map.contains(id)) return nullptr;
  return cmd_map.at(id).get();
}


const aer::Command *aer::CommandRegistry::find_command(unsigned int id) const {
  if (!cmd_map.contains(id)) return nullptr;
  return cmd_map.at(id).get();
}
