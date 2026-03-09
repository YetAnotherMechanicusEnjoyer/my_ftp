#include "Commands.hpp"
#include "Server.hpp"
#include <cctype>
#include <algorithm>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <stdexcept>
#include <cstring>
#include <sstream>

namespace ftp {
Server::Server(int port, const std::string &path) : _port(port), _defaultPath(path) {
  initSocket();
  initCommands();
}

void Server::initCommands() {
  _cmds.insert({"USER", [&](Client& client, const std::string& args) { return cmdUser(client, args); } });
  _cmds.insert({"PASS", [&](Client& client, const std::string& args) { return cmdPass(client, args); } });
  _cmds.insert({"QUIT", [&](Client& client, const std::string& args) { return cmdQuit(client, args); } });
  _cmds.insert({"HELP", [&](Client& client, const std::string& args) { return cmdHelp(client, args); } });
  _cmds.insert({"CDUP", [&](Client& client, const std::string& args) { return cmdCdup(client, args); } });
  _cmds.insert({"CWD", [&](Client& client, const std::string& args) { return cmdCwd(client, args); } });
  _cmds.insert({"DELE", [&](Client& client, const std::string& args) { return cmdDele(client, args); } });
  _cmds.insert({"NOOP", [&](Client& client, const std::string& args) { return cmdNoop(client, args); } });
  _cmds.insert({"PWD", [&](Client& client, const std::string& args) { return cmdPwd(client, args); } });
}

void Server::initSocket() {
  int fd = socket(AF_INET, SOCK_STREAM, 0);
  if (fd == -1) throw std::runtime_error("Failed to create socket");

  int opt = 1;
  setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

  struct sockaddr_in addr{};
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = INADDR_ANY;
  addr.sin_port = htons(_port);

  if (bind(fd, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
    ::close(fd);
    throw std::runtime_error("Failed to bind socket");
  }

  if (listen(fd, SOMAXCONN) == -1) {
    ::close(fd);
    throw std::runtime_error("Failed to listen on socket");
  }

  _serverSocket.setFd(fd);

  struct pollfd pfd{};

  pfd.fd = _serverSocket.getFd();
  pfd.events = POLLIN;
  _pollFds.push_back(pfd);
}

void Server::run() {
  std::cout << "Server listening on port " << _port << "..." << std::endl;

  while (true) {
    if (poll(_pollFds.data(), _pollFds.size(), -1) == -1) {
      throw std::runtime_error("Poll failed");
    }

    for (size_t i = 0; i < _pollFds.size(); ++i) {
      if (_pollFds[i].revents & POLLIN) {
        if (_pollFds[i].fd == _serverSocket.getFd()) {
          acceptNewClient();
        } else {
          handleClientData(_pollFds[i].fd);
        }
      }
    }
  }
}

void Server::acceptNewClient() {
  struct sockaddr_in clientAddr;
  socklen_t clientLen = sizeof(clientAddr);
  int clientFd = accept(_serverSocket.getFd(), (struct sockaddr*)&clientAddr, &clientLen);

  if (clientFd != -1) {
    struct pollfd pfd{};
    pfd.fd = clientFd;
    pfd.events = POLLIN;
    _pollFds.push_back(pfd);

    _clients.emplace(clientFd, Client(clientFd, _defaultPath));
    _clients.at(clientFd).sendMessage("220 Service ready for new user.");
    std::cout << "New client connected." << std::endl;
  }
}

void Server::handleClientData(int fd) {
  char buffer[1024];
  std::memset(buffer, 0, sizeof(buffer));
  ssize_t bytesRead = ::read(fd, buffer, sizeof(buffer) - 1);

  if (bytesRead <= 0) {
    std::cout << "Client disconnected." << std::endl;
    for (auto it = _pollFds.begin(); it != _pollFds.end(); ++it) {
      if (it->fd == fd) {
        _pollFds.erase(it);
        break;
      }
    }
    _clients.erase(fd);
  } else {
    Client& client = _clients.at(fd);
    client.appendToBuffer(buffer);

    std::string cmd;
    while((cmd = client.extractCommand()) != "") {
      processCommand(client, cmd);
    }
  }
}

void Server::processCommand(Client& client, const std::string& command) {
  std::istringstream iss(command);
  std::string cmd;
  iss >> cmd;

  std::transform(cmd.begin(), cmd.end(), cmd.begin(), ::toupper);
  std::string args;
  std::getline(iss, args);
  if(!args.empty() && args[0] == ' ') args.erase(0, 1);
  if(!args.empty() && args.back() == '\r') args.pop_back();

  std::cout << "Received: [" << cmd << "] with args: [" << args << "]" << std::endl;

  auto it = _cmds.find(cmd);
  if (it == _cmds.end()) {
    client.sendMessage("500 Syntax error, command unrecognized.");
  } else {
    if (!client.isLogged() && cmd != "USER" && cmd != "PASS" && cmd != "QUIT") {
      client.sendMessage("530 Not logged in.");
      return;
    }
    it->second(client, args);
  }
}
}
