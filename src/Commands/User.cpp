#include "Commands.hpp"

namespace ftp {
void cmdUser(Client& client, const std::string& args) {
  if (args.empty()) {
    client.sendMessage("530 Permission denied.");
    return;
  }
  client.setUsername(args);
  client.sendMessage("331 User name okay, need password.");
}
}
