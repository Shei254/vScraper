#pragma once
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>

struct HTMLNode {
	std::string tagName;
	std::string textContext;
	std::unordered_map<std::string, std::string> attributes;
	std::vector<std::shared_ptr<HTMLNode>> children;
	HTMLNode* parent;

	explicit HTMLNode(const std::string& name, HTMLNode* parentNode = nullptr) : tagName(name), parent(parentNode) {}
};