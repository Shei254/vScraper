#pragma once
#include <optional>
#include <variant>
#include "HTMLParserContext.h"


struct TextState;
struct TagState
{
	void update(HTMLParserContext& context);
};

