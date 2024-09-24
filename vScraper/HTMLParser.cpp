#include <iostream>
#include "HTMLParser.h"

#include "FSM.h"
#include "HTMLParserContext.h"
#include "TagState.h"
#include "ReadTagState.h"
#include "TextState.h"
#include "AttributeState.h"


using StateVariant = std::variant<TagState, ReadTagState, TextState, AttributeState>;
using OptionalStateVariant = std::optional<StateVariant>;

OptionalStateVariant transition(TagState& tagState, HTMLParserContext& context);
OptionalStateVariant transition(ReadTagState& tagState, HTMLParserContext& context);
OptionalStateVariant transition(TextState& textState, HTMLParserContext& context);
OptionalStateVariant transition(AttributeState& attributeState, HTMLParserContext& context);

void HTMLParser::printTree(const std::shared_ptr<HTMLNode>& node, int depth = 0)
{
	for (int i = 0; i < depth; i++) {
		std::cout << " ";
	}
	std::cout << node->tagName << " : " << node->textContext << "\n";

	for (auto& child : node->children) {
		printTree(child, depth + 1);
	}
}

void HTMLParser::parse(std::string& html)
{
	FSM<HTMLParserContext, TagState, ReadTagState, TextState, AttributeState> fsm{ TagState{}, std::move(HTMLParserContext{html}) };

	while (!fsm.context().isEnd()) {
		fsm.update();
	}

	printTree(fsm.context().getRootNode());
}

OptionalStateVariant transition(TagState& tagState, HTMLParserContext& context)
{
	if (!context.isEnd() && context.currentChar() == '<') {
		std::cout << "Transitioning From TagState To ReadTagState" << std::endl;
		context.advance(); //Skip the < character

		return ReadTagState{};
	}

	return std::nullopt;
}

OptionalStateVariant transition(ReadTagState& readTagState, HTMLParserContext& context)
{
	if (!context.isEnd()) {
		if (context.currentChar() == ' ') {
			std::cout << "Transitioning From ReadTagState To AttributeState" << std::endl;
			
			context.advance(); //Skip The Space
			return AttributeState{};
		}
		else if (context.currentChar() == '/' && context.peek() == '>') {
			//Handle Self Closing Tags

			context.advance(2); // skip both / & >
			context.moveToParentNode(); // move back to parent after closing tag
			return TagState{};
		}
		else if (context.currentChar() == '>') {
			context.advance(); // skip >
			return TextState{};
		}
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
	if (!context.isEnd()) {
		//Handle Self Closing Tags
		if (context.currentChar() == '/' && context.peek() == '>') context.advance();
		
		if (context.currentChar() == ' ' || context.currentChar() == '>') {
			std::cout << "Transitioning from AttrState To ReadTag State" << std::endl;
			return ReadTagState{};
		}
	}
	return std::nullopt;
}
