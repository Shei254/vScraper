#include <iostream>
#include <string>
#include <chrono>

#include "HTMLParser.h"

int main() {
	std::string html = "<html><body class='example-class'>Hello, World!<div><p>shihead</p></div><p>shithead</p></body></html>";
	
	HTMLParser parser;
	auto start = std::chrono::high_resolution_clock::now();
	parser.parse(html);
	auto end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> duration = end - start;

	std::cout << "Parsing time: " << duration.count() << " seconds" << std::endl;
}