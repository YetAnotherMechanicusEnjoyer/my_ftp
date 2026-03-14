#ifndef CLIENT
#define CLIENT

#include "TCPSocket.hpp"
#include <string>
#include <netinet/in.h>

namespace ftp {
class Client {
public:
  Client(int fd, const std::string& defaultDir);
  ~Client();

  Client(Client&&) noexcept = default;
  Client& operator=(Client&&) noexcept = default;

  enum TransferMode {
    NONE,
    ACTIVE,
    PASSIVE,
  };

  int getFd() const;
  void appendToBuffer(const std::string& data);
  std::string extractCommand();

  bool isLogged() const;
  void setLogged(bool status);
  void setUsername(const std::string& user);
  std::string getUsername() const;

  const std::string& getCurrentDir() const;
  void setCurrentDir(const std::string& path);

  void setTransferMode(TransferMode mode);
  TransferMode getTransferMode() const;
  void setDataAddr(const struct sockaddr_in& addr);
  const struct sockaddr_in& getDataAddr() const;
  void setDataFd(int fd);
  int getDataFd() const;

  void sendMessage(const std::string& msg) const;

private:
  TCPSocket _socket;
  std::string _buffer;
  std::string _currentDir;
  bool _isLogged;
  std::string _username;
  TransferMode _mode;
  int _dataFd;
  struct sockaddr_in _dataAddr;

};
}

#endif /* CLIENT */
