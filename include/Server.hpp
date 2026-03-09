#ifndef SERVER
#define SERVER

#include "TCPSocket.hpp"
#include "Client.hpp"
#include <functional>
#include <vector>
#include <map>
#include <poll.h>
#include <string>

namespace ftp {
class Server {
private:
  using Command = std::map<std::string, std::function<void(Client&, const std::string&)>>;
  int _port;
  std::string _defaultPath;
  TCPSocket _serverSocket;
  std::vector<struct pollfd> _pollFds;
  std::map<int, Client> _clients;
  Command _cmds;

  void initSocket();
  void initCommands();
  void acceptNewClient();
  void handleClientData(int fd);
  void processCommand(Client& client, const std::string& command);

public:
  Server(int port, const std::string& path);
  ~Server() = default;

  void run();
};
}

#endif /* SERVER */
