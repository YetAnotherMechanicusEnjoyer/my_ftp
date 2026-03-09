#include <iostream>
#include <ostream>
#include "Core.hpp"
#include "Errors.hpp"
#include "Server.hpp"

static void printUsage(std::ostream& cstream) {
  cstream <<
    "USAGE: ./myftp port path" << std::endl <<
    " port is the port number on which the server socket listens" << std::endl <<
    " path is the path to the home directory for the Anonymous user" << std::endl;
}

static unsigned int handle_args(int argc, const char *argv[]) {
  if (argc != 3 || !argv || !argv[1] || !argv[2]) {
    printUsage(std::cerr);
    return ERROR;
  }
  std::string flag(argv[1]);
  if (argc == 2 && flag == "--help" || flag == "-h") {
    printUsage(std::cout);
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
        ftp::Server ftpServer(std::stoi(argv[1]), argv[2]);

        ftpServer.run();
        return SUCCESS;
      } catch (const ftp::FTPError& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return ERROR;
      } catch (...) {
        std::cerr << "Uncaught Error." << std::endl;
        return ERROR;
      }
  }
}
