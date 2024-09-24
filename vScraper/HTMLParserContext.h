#pragma once
#include <string>
#include <memory>
#include "HtmlNode.h"


class HTMLParserContext
{
private:
	std::string _input;
	size_t _position = 0;

	std::shared_ptr<HTMLNode> rootNode;
	HTMLNode* currentNode;

public:
	HTMLParserContext(std::string html) : _input{ std::move(html) }, _position{ 0 }
	{
		std::string rootTag = "root";
		rootNode = std::make_shared<HTMLNode>(rootTag);
		currentNode = rootNode.get();
	}

	void addChildNode(const std::string& tagName) {
		auto newNode = std::make_shared<HTMLNode>(tagName, currentNode);
		currentNode->children.push_back(newNode);
		currentNode = newNode.get();
	}

	void moveToParentNode() {
		if (currentNode->parent != nullptr) {
			currentNode = currentNode->parent;
		}
	}

	char currentChar()
	{
		return _input[_position];
	}

	void advance(int offset = 1)
	{
		_position += offset;
	}

	bool isEnd()
	{
		return _position >= _input.size();
	}

	char peek(int offset = 1) {
		return (_position + offset < _input.size() ? _input[_position + offset] : '\0');
	}

	std::shared_ptr<HTMLNode> getRootNode() {
		return rootNode;
	}

	HTMLNode* getCurrentNode() {
		return currentNode;
	}

	int getCurreontPosition() {
		return _position;
	}
};

