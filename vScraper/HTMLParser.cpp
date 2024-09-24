#include <iostream>
#include "HTMLParser.h"

#include "FSM.h"
#include "HTMLParserContext.h"
#include "TagState.h"
#include "Transitions.h"


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
	HTMLParserFSM fsm{ TagState{}, std::move(HTMLParserContext{html}) };

	while (!fsm.context().isEnd()) {
		fsm.update();
	}

	printTree(fsm.context().getRootNode());
}