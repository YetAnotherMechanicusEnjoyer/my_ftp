#include "Commands.hpp"

namespace ftp {
void cmdHelp(Client &client, const std::string &args) {
  (void)args;
  client.sendMessage("214-The following commands are recognized.");
  client.sendMessage(" USER PASS QUIT CWD CDUP PWD DELE HELP NOOP PASV PORT LIST RETR STOR");
  client.sendMessage("214 Help OK.");
}
}
