#pragma once
#include <string>
#include "TCPCommon.h"

#ifdef _WIN32
typedef void (*HANDLER)(SOCKET sock);
#else 
typedef void (*HANDLER)(int sock);
#endif

class TCPClient
{
private:
#ifdef _WIN32
	SOCKET _sock;
#else
	int _sock;
#endif
	std::string _host = nullptr;
	std::string _port = nullptr;
	
public:
	explicit TCPClient(std::string &host, std::string &port);	
	void makeConnection();
	
	int getSock();
	std::string getHost();
	std::string getPort();
};

