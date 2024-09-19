#include <iostream>
#include <stdio.h>
#include <malloc.h>
#include <type_traits>

#include "AVL.h"

template<typename T>
AVL<T>::AVL(T_COMPARATOR comparator) : _comparator(comparator), _root(nullptr)
{

}

template<typename T>
void AVL<T>::insert(T data)
{
	int dataLen = 0;
	if  (std::is_same_v<T, std::string> == true) {
		dataLen = data.length();
	}
	else if (std::is_same_v<T, const char*> == true) {
		dataLen = strlen(data);
	}
	else {
		dataLen = sizeof(data);
	}


	struct AVLNode<T>* newNode = (struct AVLNode<T>*)calloc(1, sizeof(struct AVLNode));
	if (!newNode) {
		fprintf(stderr, "[-] error allocating memory for a new node\n");
		exit(-1);
	}

	newNode->height = 1;
	newNode->data = calloc(1, dataLen);
	memcpy(newNode->data, data, dataLen);

	if (this->_root == nullptr) {
		this->_root = newNode;
		return;
	}

	int direction = 2;
	struct AVLNode<T>* cursor = iterate(data, direction, this->_root);
	if (direction == 0) {
		std::cout << "Item already exists" << std::endl;
		return;
	}

	if (direction == -1) {
		cursor->left = newNode;
	}
	else {
		cursor->right = newNode;
	}

	cursor->height = calculateNodeHeight(cursor);

	if (calculateBalanceFactor(cursor) == 2 && calculateBalanceFactor(cursor->left) == 1) {
		ll_rotation(cursor);
	}
	else if (calculateBalanceFactor(cursor) == 2 && calculateBalanceFactor(cursor->left) == -1) {
		lr_rotation(cursor);
	}
	else if (calculateBalanceFactor(cursor) == 2 && calculateBalanceFactor(cursor->right) == -1) {
		rr_rotation(cursor);
	}
	else if (calculateBalanceFactor(cursor) == 2 && calculateBalanceFactor(cursor->right) == 1) {
		rl_rotation(cursor);
	}
}


template<typename T>
void AVL<T>::remove(T data)
{
	int direction = 2;
	struct AVLNode<T>* cursor = iterate(data, direction, this->_root);

	if (direction != 0) {
		std::cout << "Item to be deleted not found\n" << std::endl;
		return;
	}

	if (cursor == this->_root) {
		free(cursor->data);
		free(cursor);
		this->_root = nullptr;
		return;
	}



}

template<typename T>
AVLNode<T>* AVL<T>::search(T data)
{
	int direction = 2;
	struct AVLNode<T>* cursor = iterate(data, direction, this->_root);
	if (direction == 0) return cursor;

	return nullptr;
}

template<typename T>
AVLNode<T>* AVL<T>::iterate(T data, int& direction, AVLNode<T>* cursor)
{
	if (cursor == nullptr) return nullptr;
	int comparison = this->_comparator(data, cursor->data);
	
	if (comparison == 0) {
		direction = 0;
	}
	else if (comparison < 0) {
		direction = -1;
		if (cursor->left) iterate(data, direction, cursor->left);
	}
	else {
		direction = 1;
		if (cursor->right) iterate(data, direction, cursor->right);
	}

	return cursor;
}

template<typename T>
int AVL<T>::calculateNodeHeight(struct AVLNode<T>* node)
{
	if (node->left == nullptr && node->right) {
		return node->right->height + 1;
	}
	else if (node->right == nullptr && node->left) {
		return node->left->height + 1;
	}
	else if (node->left && node->right) {
		return std::max(node->left->height, node->right->height) + 1;
	}

	return 0;
}

template<typename T>
int AVL<T>::calculateBalanceFactor(AVLNode<T>* node)
{
	if (node->left && node->right) {
		return node->left->height - node->right->height;
	}
	else if (node->left == nullptr && node->right) {
		return node->right->height;
	}
	else if (node->right == nullptr && node->left) {
		return node->left->height;
	}

	return 0;
}

template<typename T>
AVLNode<T>* AVL<T>::rr_rotation(AVLNode<T>* node)
{
	struct AVLNode<T>* p = node;
	struct AVLNode<T>* tmp = p->right;

	p->right = tmp->left;
	tmp->left = p;

	return tmp;
}

template<typename T>
AVLNode<T>* AVL<T>::rl_rotation(AVLNode<T>* node)
{
	struct AVLNode<T>* p = node;
	struct AVLNode<T>* tmp = p->right;
	struct AVLNode<T>* tmp2 = tmp->left;


	p->right = tmp2->left;
	tmp->left = tmp2->right;
	tmp2->left = p;
	tmp2->right = tmp;

	return tmp2;
}

template<typename T>
AVLNode<T>* AVL<T>::ll_rotation(AVLNode<T>* node)
{
	struct AVLNode<T>* p = node;
	struct AVLNode<T>* tmp = p->left;

	p->left = tmp->right;
	tmp->right = p;

	return tmp;
}

template<typename T>
AVLNode<T>* AVL<T>::lr_rotation(AVLNode<T>* node)
{
	struct AVLNode<T>* p = node;
	struct AVLNode<T>* tmp = p->right;

	p->right = tmp->right;
	tmp->left = p;

	return tmp;
}

template<typename T>
AVLNode<T>* AVL<T>::inpre(AVLNode<T>* node)
{
	struct AVLNode<T>* p = node->right;
	while (p && p->right != NULL) {
		p = p->right;
	}
	return p;
}

template<typename T>
AVLNode<T>* AVL<T>::insuc(AVLNode<T>* node)
{
	struct AVLNode<T>* p = node->left;
	while (p && p->left) {
		p = p->left;
	}
	return p;
}

template<typename T>
AVLNode<T>* AVL<T>::removeNode(T data, AVLNode<T>* cursor)
{
	if (cursor->left == nullptr && cursor->right == nullptr) {
		if (cursor == _root) {
			free(cursor->data);
			free(cursor);
			_root = nullptr;

			return nullptr;
		}
	}

	struct AVLNode<T>* t, *q;
	if (_comparator(data, cursor->data) == -1) cursor->right = removeNode(data, cursor->left);
	else if (_comparator(data, cursor->data) == 1) cursor->left = removeNode(data, cursor->right);
	else {
		if (cursor->left != nullptr) {
			q = inpre(cursor->left);
			cursor->data = q->data;
			cursor->left = removeNode(data, cursor->left);
		}
		else {
			q = insuc(cursor->right);
			cursor->data = q->data;
			cursor->right = removeNode(data, cursor->right);
		}


		if (calculateBalanceFactor(cursor) == 2 && calculateBalanceFactor(cursor->left) == 1) {
			cursor = ll_rotation(cursor);
		}
		else if (calculateBalanceFactor(cursor) == 2 && calculateBalanceFactor(cursor->left) == -1) {
			cursor = lr_rotation(cursor);
		}
		else if (calculateBalanceFactor(cursor) == 2 && calculateBalanceFactor(cursor->left) == 0) {
			cursor = ll_rotation(cursor);
		}
		else if (calculateBalanceFactor(cursor) == 2 && calculateBalanceFactor(cursor->right) == 1) {
			cursor = rl_rotation(cursor);
		}
		else if (calculateBalanceFactor(cursor) == 2 && calculateBalanceFactor(cursor->right) == -1) {
			cursor = rr_rotation(cursor);
		}
		else if (calculateBalanceFactor(cursor) == 2 && calculateBalanceFactor(cursor->right) == 0) {
			cursor = ll_rotation(cursor);
		}
	}
	return cursor;
}
