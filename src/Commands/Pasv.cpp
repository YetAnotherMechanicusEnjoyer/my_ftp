#include "Commands.hpp"
#include <cstdint>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

namespace ftp {
void cmdPasv(Client &client, const std::string &args) {
  (void)args;
  int listenFd = socket(AF_INET, SOCK_STREAM, 0);
  if (listenFd == -1) {
    client.sendMessage("425 Can't open data connection.");
    return;
  }

  struct sockaddr_in addr{};
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = INADDR_ANY;
  addr.sin_port = 0;

  if (bind(listenFd, (struct sockaddr*)&addr, sizeof(addr)) == 1 || listen(listenFd, 1) == -1) {
    ::close(listenFd);
    client.sendMessage("425 Can't open data connection.");
    return;
  }

  socklen_t len = sizeof(addr);
  getsockname(listenFd, (struct sockaddr*)&addr, &len);

  uint32_t ip = ntohl(inet_addr("127.0.0.1"));
  uint16_t port = ntohs(addr.sin_port);

  char response[100];
  std::sprintf(response, "227 Entering Passive Mode (%d,%d,%d,%d,%d,%d).",
               (ip >> 24) & 0xFF, (ip >> 16) & 0xFF, (ip >> 8) & 0xFF, ip & 0xFF, (port >> 8) & 0xFF, port & 0xFF);
  client.sendMessage(response);
}
}
