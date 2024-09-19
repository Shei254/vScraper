#include <iostream>
#include <string>

#include "HTMLParserContext.h"
#include "TextState.h"
#include "TagState.h"
#include "FSM.h"

using SteteVariant = std::variant<TextState, TagState>;
using OptionalStateVariant = std::optional<SteteVariant>;

OptionalStateVariant transition(TagState& state, HTMLParserContext& context) {
	if (!context.isEnd() && context.currentChar() == '>') {
		std::cout << "Transitioning to TagState" << std::endl;
		context.advance(); //move past >
		return OptionalStateVariant{ TextState{} };
	}
	return std::nullopt;
}

OptionalStateVariant transition(TextState& state, HTMLParserContext& context) {
	if (!context.isEnd() && context.currentChar() == '<') {
		std::cout << "Transitioning to TextState" << std::endl;
		return OptionalStateVariant{ TagState{} };
	}
	return std::nullopt;
}

int main() {
	std::string html = "<html><body>Hello, World!</body></html>";

	static FSM<HTMLParserContext, TextState, TagState> fsm{ TagState{}, std::move(HTMLParserContext{html}) };

	while (!fsm.context().isEnd()) {
		fsm.update();
	}
}