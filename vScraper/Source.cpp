#include <iostream>
#include <string>

#include "HTMLParser.h"

int main() {
	std::string html = "<html><body class='example-class'>Hello, World!</body></html>";
	
	HTMLParser parser;
	parser.parse(html);
}