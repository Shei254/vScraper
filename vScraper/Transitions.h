#pragma once
#include "TagState.h"
#include "ReadTagState.h"
#include "TextState.h"
#include "AttributeState.h"
#include "CloseTagState.h"

using HTMLParserFSM = FSM<HTMLParserContext, TagState, ReadTagState, TextState, AttributeState, CloseTagState>;
using StateVariant = std::variant<TagState, ReadTagState, TextState, AttributeState, CloseTagState>;
using OptionalStateVariant = std::optional<StateVariant>;

OptionalStateVariant transition(TagState& tagState, HTMLParserContext& context)
{
	if (!context.isEnd() && context.currentChar() == '<') {
		context.advance(); // Skip the < character
		if (context.currentChar() == '/') {
			std::cout << "Transitioning From TagState To CloseTagState" << std::endl;
			return CloseTagState{};
		}

		std::cout << "Transitioning From TagState To ReadTagState" << std::endl;
		return ReadTagState{};
	}
	return std::nullopt;
}

OptionalStateVariant transition(ReadTagState& readTagState, HTMLParserContext& context)
{
	if (!context.isEnd()) {
		if (context.currentChar() == ' ') {
			std::cout << "Transitioning From ReadTagState To AttributeState" << std::endl;
			context.advance(); // Skip the space
			return AttributeState{};
		}
	
		else if ((context.currentChar() == '/' && context.peek() == '>') || context.currentChar() == '>') {
			// Handle self-closing or end of tag
			if (context.currentChar() == '/') {
				context.advance(2); // Skip '/>'
			}
			else {
				context.advance(); // Skip '>'
			}

			if (!context.isEnd() && context.currentChar() == '<') {
				std::cout << "Transitioning From AttributeTagState To TagState" << std::endl;
				return TagState{}; // Start a new tag if the next character is '<'
			}

			std::cout << "Transitioning From AttributeState To TextState" << std::endl;
			return TextState{}; // Otherwise, continue with text content
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
	if (!context.isEnd() && (context.currentChar() == '>' || context.currentChar() == ' ' || (context.currentChar() == '/' && context.peek() == '>'))) {
		std::cout << "Transitioning from AttributeState To ReadTagState" << std::endl;
		return ReadTagState{};
	}
	return std::nullopt;
}

OptionalStateVariant transition(CloseTagState& closeTagState, HTMLParserContext& context)
{
	if (!context.isEnd() && context.currentChar() == '>') {
		// Advance past '>'
		context.advance();
		context.moveToParentNode(); // Move back to parent node if necessary
		if (context.currentChar() == '<') {
			std::cout << "Transitioning from CloseTagState to TagState" << std::endl;
			return TagState{};
		}
		else {
			std::cout << "Transitioning from CloseTagState to TextState" << std::endl;
			return TextState{};
		}
	}
	return std::nullopt; // Handle cases with no transition
}
