#include "ReadTagState.h"

void ReadTagState::update(HTMLParserContext& context)
{
	std::string tagName;
	while (!context.isEnd()) {
		if (context.currentChar() == ' ' || context.currentChar() == '>' || (context.currentChar() == '/' && context.peek() == '>')) break;

		tagName += context.currentChar();
		context.advance();
	}

	if (!tagName.empty()) {
		context.addChildNode(tagName);
	}
}
