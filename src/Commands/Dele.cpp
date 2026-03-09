#include "Commands.hpp"
#include <cstdio>

namespace ftp {
void cmdDele(Client &client, const std::string &args) {
  if (args.empty()) {
    client.sendMessage("550 File not found.");
    return;
  }

  std::string target = client.getCurrentDir() + "/" + args;

  if (std::remove(target.c_str()) == 0)
    client.sendMessage("250 Requested file action okay, completed.");
  else
    client.sendMessage("550 Requested action not taken. File unavailable.");
}
}
