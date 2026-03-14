#include "Commands.hpp"

namespace ftp {
void cmdCwd(Client &client, const std::string &args) {
  if (args.empty()) {
    client.sendMessage("550 Failed to change directory.");
    return;
  }

  std::string newDir = args;
  if (newDir[0] != '/') {
    if (client.getCurrentDir().back() != '/')
      newDir = client.getCurrentDir() + "/" + newDir;
    else
      newDir = client.getCurrentDir() + newDir;
  }

  client.setCurrentDir(newDir);
  client.sendMessage("250 Requested file action okay, completed.");
}
}
