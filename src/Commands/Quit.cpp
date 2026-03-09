#include "Commands.hpp"

namespace ftp {
void cmdQuit(Client& client, const std::string&) {
  client.sendMessage("221 Service closing control connection.");
}
}
