#pragma once
#include <memory>
#include <optional>
#include <variant>

#include "HTMLParserContext.h"

class State {
public:
	virtual ~State() = default;
	virtual void update(HTMLParserContext& context) = 0;
	virtual std::optional<std::unique_ptr<State>> transition(HTMLParserContext& context) = 0;
};