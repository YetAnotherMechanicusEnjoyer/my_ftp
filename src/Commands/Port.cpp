#include "Commands.hpp"
#include <vector>
#include <sstream>
#include <arpa/inet.h>

namespace ftp {
void cmdPort(Client &client, const std::string &args) {
  std::vector<std::string> tokens;
  std::stringstream ss(args);
  std::string item;

  while (std::getline(ss, item, ',')) tokens.push_back(item);

  if (tokens.size() != 6) {
    client.sendMessage("501 Syntax error in parameters or arguments.");
    return;
  }

  std::string ip = tokens[0] + "." + tokens[1] + "." + tokens[2] + "." + tokens[3];
  int port = (std::stoi(tokens[4]) << 8) | std::stoi(tokens[5]);

  struct sockaddr_in addr{};
  addr.sin_family = AF_INET;
  addr.sin_port = htons(port);
  if (inet_pton(AF_INET, ip.c_str(), &addr.sin_addr) <= 0) {
    client.sendMessage("501 Invalid address.");
    return;
  }

  client.setDataAddr(addr);
  client.setTransferMode(Client::ACTIVE);
  client.sendMessage("200 PORT command successful.");
}
}
