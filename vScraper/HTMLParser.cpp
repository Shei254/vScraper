#include <iostream>
#include "HTMLParser.h"

#include "FSM.h"
#include "HTMLParserContext.h"
#include "TagState.h"
#include "TextState.h"
#include "AttributeState.h"

using StateVariant = std::variant<TagState, TextState, AttributeState>;
using OptionalStateVariant = std::optional<StateVariant>;

OptionalStateVariant transition(TagState& tagState, HTMLParserContext& context);
OptionalStateVariant transition(TextState& textState, HTMLParserContext& context);
OptionalStateVariant transition(AttributeState& attributeState, HTMLParserContext& context);

void HTMLParser::parse(std::string& html)
{
	static FSM<HTMLParserContext, TagState, TextState, AttributeState> fsm{ TagState{}, std::move(HTMLParserContext{html}) };

	while (!fsm.context().isEnd()) {
		fsm.update();
	}
}

OptionalStateVariant transition(TagState& tagState, HTMLParserContext& context)
{
	if (!context.isEnd() && context.currentChar() == '>') {
		std::cout << "Transitioning From TagState To TextState" << std::endl;
		context.advance(); //move past >
		return TextState{};
	}

	if (!context.isEnd() && context.currentChar() == ' ') {
		std::cout << "Transitioning From TagState To AttrState" << std::endl;
		return AttributeState{};
	}

	return std::nullopt;
}

OptionalStateVariant transition(TextState& textState, HTMLParserContext& context)
{
	if (!context.isEnd() && context.currentChar() == '<') {
		std::cout << "Transitioning From TextState to TagState" << std::endl;
		return TagState{};
	}

	return std::nullopt;
}

OptionalStateVariant transition(AttributeState& attributeState, HTMLParserContext& context)
{
	if (!context.isEnd() && context.currentChar() == '>') {
		std::cout << "Transitioning from Attr State To Tag State" << std::endl;
		return TagState{};
	}
	return OptionalStateVariant();
}
