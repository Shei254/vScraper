#include "AsyncNetIOWin.h"

void AsyncNetIOWin::init()
{
	hCompletionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
}

void AsyncNetIOWin::handleAsyncIO()
{
	for (;;) {
		DWORD bytesTransffered;
		ULONG_PTR completionKey;
		LPOVERLAPPED overlapped;

		if (GetQueuedCompletionStatus(hCompletionPort, &bytesTransffered, &completionKey, &overlapped, INFINITE)) {
			struct Connection* connection = reinterpret_cast<struct Connection*>(completionKey);
			if (bytesTransffered > 0) {
				//handleIOCompletion(connection);
			}
			else {
				std::cout << "Connection close" << std::endl;
				closesocket(connection->socket);
				delete connection;
			}
		}
	}
}

void AsyncNetIOWin::asyncSend(Connection* connection)
{
	WSABUF buf = { 0 };
	buf.buf = (char*)connection->_writeBuffer.c_str();
	buf.len = connection->_writeBuffer.length();
	
	DWORD bytesSent = 0;
	
	OVERLAPPED overlapped;
	WSASend(connection->socket, &buf, 1, &bytesSent, 0, &overlapped, NULL);
	connection->status = WRITE;
}

void AsyncNetIOWin::asyncRead(Connection* connection)
{
	WSABUF buf = { 0 };
	buf.buf = (char*)connection->_readBuffer.c_str();
	buf.len = connection->_readBuffer.length();

	DWORD bytesRcvd = 0, flags = 0;

	OVERLAPPED overlapped;
	WSARecv(connection->socket, &buf, 1, NULL, &flags, &overlapped, NULL);
	connection->status = READ;
}

AsyncNetIO* createAsyncIO()
{
	return new AsyncNetIOWin();
}
