#ifndef COMMAND
#define COMMAND
#include "Client.hpp"
#include <string>

namespace ftp {
void cmdUser(Client& client, const std::string& args);
void cmdPass(Client& client, const std::string& args);
void cmdQuit(Client& client, const std::string&);
void cmdPwd(Client &client, const std::string &args);
void cmdCwd(Client &client, const std::string &args);
void cmdCdup(Client &client, const std::string &args);
void cmdDele(Client &client, const std::string &args);
void cmdHelp(Client &client, const std::string &args);
void cmdNoop(Client &client, const std::string &args);
void cmdPort(Client &client, const std::string &args);
void cmdPasv(Client &client, const std::string &args);
void cmdList(Client &client, const std::string &args);
void cmdRetr(Client &client, const std::string &args);
void cmdStor(Client &client, const std::string &args);
}

#endif /* COMMAND */
