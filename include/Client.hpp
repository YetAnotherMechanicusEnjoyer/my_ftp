#ifndef CLIENT
#define CLIENT

#include "TCPSocket.hpp"
#include <string>

namespace ftp {
class Client {
private:
  TCPSocket _socket;
  std::string _buffer;
  std::string _currentDir;
  bool _isLogged;
  std::string _username;

public:
  Client(int fd, const std::string& defaultDir);
  ~Client() = default;

  Client(Client&&) noexcept = default;
  Client& operator=(Client&&) noexcept = default;

  int getFd() const;
  void appendToBuffer(const std::string& data);
  std::string extractCommand();

  bool isLogged() const;
  void setLogged(bool status);
  void setUsername(const std::string& user);
  std::string getUsername() const;

  std::string getCurrentDir() const;
  void sendCurrentDir(const std::string path);

  void sendMessage(const std::string& msg) const;
};
}

#endif /* CLIENT */
