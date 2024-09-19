#pragma once
#include <string>

struct HTMLParserContext
{
	std::string _input;
	size_t _position = 0;

	HTMLParserContext(std::string html) : _input{ std::move(html) }, _position{ 0 }
	{
	}

	char currentChar()
	{
		return _input[_position];
	}

	void advance()
	{
		++_position;
	}

	bool isEnd()
	{
		return _position >= _input.size();
	}
};

