#include "TCPSocket.hpp"

namespace ftp {
TCPSocket::TCPSocket(int fd) : _fd(fd) {}

TCPSocket::~TCPSocket() {
  close();
}

TCPSocket::TCPSocket(TCPSocket&& other) noexcept : _fd(other._fd) {
  other._fd = -1;
}

TCPSocket& TCPSocket::operator=(TCPSocket&& other) noexcept {
  if (this != &other) {
    close();
    _fd = other._fd;
    other._fd = -1;
  }
  return *this;
}

void TCPSocket::close() {
  if (_fd != -1) {
    ::close(_fd);
    _fd = -1;
  }
}

int TCPSocket::getFd() const {
  return _fd;
}

void TCPSocket::setFd(int fd) {
  close();
  _fd = fd;
}
}
