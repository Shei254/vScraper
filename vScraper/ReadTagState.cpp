#include "ReadTagState.h"

void ReadTagState::update(HTMLParserContext& context)
{
	std::string tagName;
	while (!context.isEnd() && context.currentChar() != ' ' && context.currentChar() != '>') {
		tagName += context.currentChar();
		context.advance();
	}
	if (!tagName.empty()) {
		context.addChildNode(tagName);
	}
}
