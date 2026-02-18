#include <iostream>
#include "Core.hpp"

static unsigned int handle_args(int argc, const char *argv[]) {
  if (argc != 3 || !argv || !argv[1] || !argv[2]) {
    std::cerr <<
      "USAGE: ./myftp port path\n port is the port number on which the server socket listens\n path is the path to the home directory for the Anonymous user" <<
      std::endl;
    return ERROR;
  }

  std::string flag(argv[1]);
  if (flag == "--help" || flag == "-h") {
    std::cout <<
      "USAGE: ./myftp port path\n port is the port number on which the server socket listens\n path is the path to the home directory for the Anonymous user" <<
      std::endl;
    return HELP;
  }
  return SUCCESS;
}

int main(int argc, const char *argv[]) {
  switch (handle_args(argc, argv)) {
    case HELP:
      return SUCCESS;
    case ERROR:
      return ERROR;
    default:
      try {
        return SUCCESS;
      } catch (...) {
        std::cerr << "Uncaught Error." << std::endl;
        return ERROR;
      }
  }
}
