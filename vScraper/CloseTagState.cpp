#include <iostream>
#include "CloseTagState.h"

void CloseTagState::update(HTMLParserContext& context)
{
	while (!context.isEnd() && context.currentChar() != '>') {	
		context.advance();
	}
}