#pragma once
#if _WIN32
#pragma comment(lib,"Ws2_32.lib")

#include <winsock2.h>
#include <ws2tcpip.h>
#elif
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#endif

class TCPCommon
{
public:
	static void destroySocket(int socket);
};

