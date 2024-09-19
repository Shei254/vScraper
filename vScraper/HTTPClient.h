#pragma once

#include <stdint.h>
#include <unordered_map>
#include "TCPClient.h"
#include "AsyncNetIO.h"

struct HTTPResponse {
	std::string status;
	std::string statusText;
	std::unordered_map<std::string, std::string> headers;

	std::string _response;
};


class HTTPClient : public TCPClient
{
private:
	static std::string _httpVersion;
	static std::string _userAgent;
	static AsyncNetIO* _io;

	std::string generateRequest(const char* path);
	void parseHeaders(std::string& response, struct HTTPResponse& httpResponse);

	virtual void handleIOCompletion(struct Connection* connection) = 0;
public:
	HTTPClient(std::string& host, std::string& port, std::string httpVersion, std::string userAagent);
	
	void makeRequest(const char* path, struct Connection* connection);
	struct HTTPResponse getResponse(struct Connection* connection);
	
};

