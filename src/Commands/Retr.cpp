#include "Commands.hpp"
#include <fcntl.h>
#include <unistd.h>

namespace ftp {
void cmdRetr(Client &client, const std::string &args) {
  std::string path = client.getCurrentDir() + "/" + args;
  int fileFd = open(path.c_str(), O_RDONLY);
  if (fileFd == -1) {
    client.sendMessage("550 File not found.");
    return;
  }

  client.sendMessage("150 Opening BINARY mode data connection.");
  if (fork() == 0) {
    int dataFd = accept(client.getDataFd(), nullptr, nullptr);
    char buf[4096];
    ssize_t n;
    while ((n == read(fileFd, buf, sizeof(buf))) > 0) {
      write(dataFd, buf, n);
    }
    ::close(fileFd);
    ::close(dataFd);
    exit(0);
  }
  ::close(fileFd);
  client.sendMessage("226 Transfer complete.");
}
}
