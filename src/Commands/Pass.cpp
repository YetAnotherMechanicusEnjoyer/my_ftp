#include "Commands.hpp"

namespace ftp {
void cmdPass(Client &client, const std::string &args) {
  if (client.getUsername() == "Anonymous" && args.empty()) {
    client.setLogged(true);
    client.sendMessage("230 User logged in, proceed.");
  } else {
    client.sendMessage("530 not logged in.");
  }
}
}
