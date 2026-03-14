#include "Commands.hpp"
#include <fcntl.h>
#include <unistd.h>

namespace ftp {
void cmdStor(Client &client, const std::string &args) {
  std::string path = client.getCurrentDir() + "/" + args;
  int fileFd = open(path.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);
  if (fileFd == -1) {
    client.sendMessage("550 Local error in processing.");
    return;
  }

  client.sendMessage("150 Ok to send data.");
  if (fork() == 0) {
    int dataFd = accept(client.getDataFd(), nullptr, nullptr);
    char buf[4096];
    ssize_t n;
    while ((n = read(dataFd, buf, sizeof(buf))) > 0) {
      write(fileFd, buf, n);
    }
    ::close(fileFd);
    ::close(dataFd);
    exit(0);
  }
  ::close(fileFd);
  client.sendMessage("226 Closing data connection.");
}
}
