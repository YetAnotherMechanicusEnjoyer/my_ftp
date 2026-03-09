#include "Client.hpp"
#include <filesystem>
#include <unistd.h>

namespace ftp {
Client::Client(int fd, const std::string& defaultDir) : _socket(fd), _currentDir(defaultDir), _isLogged(false) {}

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

std::string Client::getCurrentDir() const { return std::filesystem::current_path(); }

void Client::sendCurrentDir(std::string path) { 
  std::string formatted = path + "\r\n";
  ::write(_socket.getFd(), formatted.c_str(), formatted.size());
}

void Client::sendMessage(const std::string& msg) const {
  std::string formatted = msg + "\r\n";
  ::write(_socket.getFd(), formatted.c_str(), formatted.size());
}
}
