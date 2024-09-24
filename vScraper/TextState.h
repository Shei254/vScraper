#pragma once
#include "HTMLParserContext.h"

struct TagState;

struct TextState {
	void update(HTMLParserContext& context);
};