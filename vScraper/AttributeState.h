#pragma once
#include "HTMLParserContext.h"

struct AttributeState {
	void update(HTMLParserContext& context) {
		if (!context.isEnd() && !context.currentChar() != '>') {
			context.advance();
		}
	}
};