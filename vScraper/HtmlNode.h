#pragma once
#include <memory>
#include <string>
#include <vector>

struct HTMLNode {
	std::string tagName;
	std::string textContext;
	std::vector<std::shared_ptr<HTMLNode>> children;
	HTMLNode* parent;

	explicit HTMLNode(const std::string& name, HTMLNode* parentNode = nullptr) : tagName(name), parent(parentNode) {}
};