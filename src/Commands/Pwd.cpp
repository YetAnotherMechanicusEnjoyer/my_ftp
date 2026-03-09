#include "Commands.hpp"

namespace ftp {
void cmdPwd(Client &client, const std::string &args) {
  (void)args;
  client.sendMessage("257 \"" + client.getCurrentDir() + "\" is the current directory.");
}
}
