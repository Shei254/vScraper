#include "TCPCommon.h"

void TCPCommon::destroySocket(int socket)
{
#if _WIN32
	closesocket((SOCKET) socket);
	WSACleanup();
#else
	close(socket);
#endif
}
