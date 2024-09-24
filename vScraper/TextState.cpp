#include <iostream>
#include "TextState.h"
#include "TagState.h"

void TextState::update(HTMLParserContext& context)
{
	std::string textContent;
	while (!context.isEnd() && context.currentChar() != '<') {
		textContent += context.currentChar();
		context.advance();
	}

	if (!textContent.empty()) {
		context.getCurrentNode()->textContext = textContent;
	}
}