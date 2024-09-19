#pragma once
#include <optional>
#include <memory>

#include "FSM.h"
#include "State.h"
#include "HTMLParserContext.h"

struct TagState
{
public:
	void update(HTMLParserContext& context) {
		while (!context.isEnd() && context.currentChar() != '>') {
			context.advance();
		}
	}
};

