#ifndef ISOCKET
#define ISOCKET

namespace ftp {
class ISocket {
public:
  virtual ~ISocket() = default;
  virtual void close() = 0;
  virtual int getFd() const = 0;
};
}

#endif /* ISOCKET */
