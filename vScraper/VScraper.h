#pragma once

#include "AsyncNetIO.h"
#include "ThreadPool.h"
#include "HTTPClient.h"


class VScraper : public HTTPClient
{
private:
	std::string _httpVersion = "HTTP/1.1";
	std::string _userAgent = "shei-browser";

	void parseResponse(std::string& response);
	void handleIOCompletion(struct Connection* connection) override;
public:
	explicit VScraper(std::string& host, std::string& port);

};

