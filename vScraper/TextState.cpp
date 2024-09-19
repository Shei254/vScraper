#include <iostream>
#include "TextState.h"
#include "TagState.h"

void TextState::update(HTMLParserContext& context)
{
	while (!context.isEnd() && context.currentChar() != '<') {
		context.advance();
	}
}