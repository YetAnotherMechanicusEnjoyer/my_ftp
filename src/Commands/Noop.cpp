#include "Commands.hpp"

namespace ftp {
void cmdNoop(Client &client, const std::string &args) {
  (void)args;
  client.sendMessage("200 NOOP ok.");
}
}
