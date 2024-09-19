#include "TCPClient.h"

TCPClient::TCPClient(std::string &host, std::string &port) : _host(std::move(host)), _port(std::move(port)), _sock(NULL)
{
#if _WIN32
	WSADATA wsa = { 0 };
	if (WSAStartup(MAKEWORD(2, 2), &wsa)) {
		fprintf(stderr, "[-] WSA Initialization failed: %d\n", WSAGetLastError());
		exit(-1);
	}
#endif
}

void TCPClient::makeConnection() 
{
	printf("[+] trying to make connection to %s:%s", _host.c_str(), _port.c_str());
	struct addrinfo hints = { 0 }, *addr = nullptr;
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;

	int gai_error = getaddrinfo(_host.c_str(), _port.c_str(), &hints, &addr);
	if (gai_error != 0) {
		if (gai_error == EAI_AGAIN) {
			fprintf(stderr, "[-] Temporary error occurred: %s. Retrying...\n", gai_strerror(gai_error));
			// Retry logic (you may want to add a delay or limit the number of retries)
			int retry_gai_error = getaddrinfo(_host.c_str(), _port.c_str(), &hints, &addr);
			if (retry_gai_error != 0) {
				fprintf(stderr, "[-] Error resolving host: %s. Error: %s\n", _host.c_str(), gai_strerror(retry_gai_error));
				exit(-1);
			}
		}
		else {
			fprintf(stderr, "[-] Error resolving host: %s. Error: %s\n", _host.c_str(), gai_strerror(gai_error));
			exit(-1);
		}
	} 

	_sock = socket(addr->ai_family, addr->ai_socktype, addr->ai_protocol);
#if _WIN32
	if (_sock == INVALID_SOCKET) {
#else
	if (_sock == -1) {
#endif
		fprintf(stderr, "[-] error creating socket\n");
		exit(-1);
	}

	if (connect(_sock, addr->ai_addr, addr->ai_addrlen)) {
		freeaddrinfo(addr);
		TCPCommon::destroySocket(_sock);
		fprintf(stderr, "[-] error making connection to host\n");
		exit(-1);
	}

	freeaddrinfo(addr);
	printf("[+] connection established successfully\n");
}


std::string TCPClient::getHost()
{
	return _host;
}

int TCPClient::getSock()
{
	return _sock;
}

std::string TCPClient::getPort()
{
	return _port;
}
