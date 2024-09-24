#include <iostream>
#include <string>

#include "HTMLParser.h"

int main() {
	std::string html = "<html><body class='example-class'>Hello, World!<div><p>shihead</p></div><p>shithead</p></body></html>";
	
	HTMLParser parser;
	parser.parse(html);
}