#include <iostream>
#include "TagState.h"
#include "TextState.h"

void TagState::update(HTMLParserContext& context)
{
	while (!context.isEnd() && context.currentChar() != '<') {
		context.advance();
	}
}