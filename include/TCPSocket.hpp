#ifndef TCPSOCKET
#define TCPSOCKET

#include "ISocket.hpp"
#include <unistd.h>

namespace ftp {
class TCPSocket : public ISocket {
private:
  int _fd;

public:
  explicit TCPSocket(int fd = -1);
  ~TCPSocket() override;

  TCPSocket(const TCPSocket&) = delete;
  TCPSocket& operator=(const TCPSocket&) = delete;
  TCPSocket(TCPSocket&& other) noexcept;
  TCPSocket& operator=(TCPSocket&& other) noexcept;

  void close() override;
  int getFd() const override;
  void setFd(int fd);
};
}

#endif /* TCPSOCKET */
