#include <format>
#include <string>
#include <sstream>
#include "HTTPClient.h"
#include "Common.h"


std::string HTTPClient::_userAgent = "DefaultUserAgent";
std::string HTTPClient::_httpVersion = "2";

AsyncNetIO* HTTPClient::_io = createAsyncIO();

HTTPClient::HTTPClient(std::string& host, std::string& port, std::string httpVersion, std::string userAagent) : TCPClient(host, port)
{
    //_httpVersion = std::move(httpVersion);
    //_userAgent = std::move(userAagent);
    //_io = createAsyncIO();
    _io->init();
    makeConnection();
}


std::string HTTPClient::generateRequest(const char* path)
{
    return std::format("GET {} {} \r\nHost: {} \r\nUser-Agent: {}\r\n\r\n", path, _httpVersion, getHost(), _userAgent);
}


void HTTPClient::parseHeaders(std::string& response, struct HTTPResponse& httpResponse)
{
    int statusLineOffset = response.find(" ");
    response = response.substr(statusLineOffset + 1);
    
    statusLineOffset = response.find(" ");
    httpResponse.status = response.substr(0, statusLineOffset);
    response = response.substr(statusLineOffset + 1);

    statusLineOffset = response.find("\r\n");
    response = response.substr(statusLineOffset + 1);

    while (true) {
        size_t headerEnd = response.find("\r\n");
        if (headerEnd == std::string::npos) break;

        size_t separatorPosition = response.find(":");
        if (separatorPosition != std::string::npos) {
            std::string key = response.substr(0, separatorPosition);
            std::string val = response.substr(separatorPosition + 1, headerEnd - separatorPosition - 1);
            httpResponse.headers.insert({ key, val });
        }
        response = response.substr(headerEnd + 2);
    }

}

void HTTPClient::makeRequest(const char* path, Connection* connection)
{
    std::string request = generateRequest(path);
    connection->_writeBuffer = std::move(request);

    _io->asyncSend(connection);
    connection->status = WRITE;
}

HTTPResponse HTTPClient::getResponse(Connection* connection)
{
    struct HTTPResponse response;
    _io->asyncRead(connection);
    
    parseHeaders(connection->_readBuffer, response);

    response._response = std::move(connection->_readBuffer);
    return response;
}
