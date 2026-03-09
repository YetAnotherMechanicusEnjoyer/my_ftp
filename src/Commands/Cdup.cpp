#include "Commands.hpp"

namespace ftp {
void cmdCdup(Client &client, const std::string &args) {
  (void)args;
  cmdCwd(client, "..");
}
}
