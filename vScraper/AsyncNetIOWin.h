#pragma once
#ifdef _WIN32

#include <winsock2.h>
#include <windows.h>
#include <iostream>
#include "AsyncNetIO.h"

class AsyncNetIOWin : public AsyncNetIO
{
private:
	HANDLE hCompletionPort;
public:
	void init() override;
	void handleAsyncIO() override;
	void asyncSend(struct Connection* connection) override;
	void asyncRead(struct Connection* connection) override;
};

AsyncNetIO* createAsyncIO();
#endif

