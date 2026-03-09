#ifndef ERRORS
#define ERRORS
#include <exception>
#include <string>

namespace ftp {
class FTPError: public std::exception {
public:
  FTPError(std::string const &message) : _message(message) {}
  const char *what() const noexcept override { return _message.c_str(); }

private:
  std::string _message;
};
}

#endif /* ERRORS */
