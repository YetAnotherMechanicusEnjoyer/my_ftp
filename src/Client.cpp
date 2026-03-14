#include "Client.hpp"
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

namespace ftp {
Client::Client(int fd, const std::string& defaultDir) : _socket(fd), _currentDir(defaultDir), _isLogged(false), _mode(NONE), _dataFd(-1) {}

Client::~Client() {
  if (_dataFd != -1) {
    ::close(_dataFd);
  }
}

int Client::getFd() const {
  return _socket.getFd();
}

void Client::appendToBuffer(const std::string& data) {
  _buffer += data;
}

std::string Client::extractCommand() {
  size_t pos = _buffer.find("\n");

  if (pos != std::string::npos) {
    std::string line = _buffer.substr(0, pos);
    _buffer.erase(0, pos + 1);

    if (!line.empty() && line.back() == '\r')
      line.pop_back();

    return line;
  }
  return "";
}

bool Client::isLogged() const { return _isLogged; }

void Client::setLogged(bool status) { _isLogged = status; }

void Client::setUsername(const std::string& user) { _username = user; }

std::string Client::getUsername() const { return _username; }

const std::string& Client::getCurrentDir() const { return _currentDir; }

void Client::setCurrentDir(const std::string& path) {
  _currentDir = path;
}

void Client::setTransferMode(Client::TransferMode mode) { _mode = mode; }

Client::TransferMode Client::getTransferMode() const { return _mode; }

void Client::setDataAddr(const struct sockaddr_in& addr) { _dataAddr = addr; }

const struct sockaddr_in& Client::getDataAddr() const { return _dataAddr; }

void Client::setDataFd(int fd) {
  if (_dataFd != -1) ::close(_dataFd);
  _dataFd = fd;
}

int Client::getDataFd() const { return _dataFd; }

void Client::sendMessage(const std::string& msg) const {
  std::string formatted = msg + "\r\n";
  ::write(_socket.getFd(), formatted.c_str(), formatted.size());
}
}
