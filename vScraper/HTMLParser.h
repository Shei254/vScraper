#pragma once
#include <string>
#include "HtmlNode.h"

class HTMLParser
{
public:
	void parse(std::string& html);
	void printTree(const std::shared_ptr<HTMLNode>& node, int depth);
};

