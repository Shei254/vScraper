#pragma once
#include "HTMLParserContext.h"

struct AttributeState {
	void update(HTMLParserContext& context) {
		while (!context.isEnd() && context.currentChar() != '>' && context.currentChar() != ' ') {
			context.advance();
		}
	}
};