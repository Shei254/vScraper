#include <iostream>
#include <sstream>
#include "VScraper.h"


VScraper::VScraper(std::string& host, std::string& port) : HTTPClient(host, port, _httpVersion, _userAgent)
{
	
}

void VScraper::handleIOCompletion(Connection* connection)
{
	if (connection->status == WRITE) {
		//Handle Response
		auto response = getResponse(connection);
		parseResponse(response._response);
		response._response.clear();

		connection->status = CONN;
	}
	else if (connection->status == READ) {
		//Handle Next Link
	}
}


void VScraper::parseResponse(std::string& response)
{
}