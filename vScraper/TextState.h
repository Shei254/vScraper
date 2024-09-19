#pragma once
#include "State.h"
#include "HTMLParserContext.h"

struct TextState {
	void update(HTMLParserContext& context) {
		while (!context.isEnd() && context.currentChar() != '<') {
			context.advance();
		}
	}
};