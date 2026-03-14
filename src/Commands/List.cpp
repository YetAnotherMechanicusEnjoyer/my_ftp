#include "Commands.hpp"
#include <unistd.h>
#include <sys/wait.h>

namespace ftp {
void cmdList(Client &client, const std::string &args) {
  if (client.getTransferMode() == Client::NONE) {
    client.sendMessage("425 Use PORT or PASV first.");
    return;
  }

  Client::TransferMode mode = client.getTransferMode();
  int listenFd = client.getDataFd();

  client.sendMessage("150 File status okay; about to open data connection.");

  pid_t pid = fork();
  if (pid == 0) {
    int dataFd = -1;
    if (mode == Client::ACTIVE) {
      dataFd = socket(AF_INET, SOCK_STREAM, 0);
      struct sockaddr_in target = client.getDataAddr();
      if (connect(dataFd, (struct sockaddr*)&target, sizeof(target)) == -1)
        exit(1);
    } else {
      dataFd = accept(listenFd, nullptr, nullptr);
    }

    if (dataFd != -1) {
      dup2(dataFd, STDOUT_FILENO);
      close(dataFd);

      execl("/bin/ls", "ls", "-l", client.getCurrentDir().c_str(), nullptr);
      exit(0);
    }
    exit(1);
  }

  client.setTransferMode(Client::NONE);
  client.sendMessage("226 Directory send OK.");
}
}
