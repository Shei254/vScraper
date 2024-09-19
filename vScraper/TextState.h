#pragma once
#include <variant>
#include <optional>
#include "State.h"
#include "HTMLParserContext.h"

struct TagState;

struct TextState {
	void update(HTMLParserContext& context);
};