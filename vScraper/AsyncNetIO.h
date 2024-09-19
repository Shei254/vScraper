#pragma once
#include <functional>
#include <string>

enum CONNECTION_STATUS {
	CONN,
	READ,
	WRITE,
	CLOSE,
};

struct Connection {
	int socket;
	CONNECTION_STATUS status;

	std::string _readBuffer;
	size_t bytesRead = 0;

	std::string _writeBuffer;
	size_t bytesWritten = 0;
};


class AsyncNetIO
{
public:
	virtual ~AsyncNetIO() = default;

	virtual void init() = 0;
	virtual void handleAsyncIO() = 0;

	virtual void asyncSend(struct Connection* connection) = 0;
	virtual void asyncRead(struct Connection* connection) = 0;
	//virtual void waitForEvents() = 0;
	//virtual void clean() = 0;
};

//Factory function
AsyncNetIO* createAsyncIO();

