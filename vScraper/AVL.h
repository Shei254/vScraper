#pragma once

template<typename T>
struct AVLNode {
	T data;
	int height;

	struct AVLNode<T>* left;
	struct AVLNode<T>* right;
};

template<typename T>
class AVL
{
	typedef int (*T_COMPARATOR)(T data1, T data2); // return -1 if data1 < data 2, return 1 if data1 > data2, return 0 if equal
private:
	struct AVLNode<T>* _root = nullptr;
	T_COMPARATOR _comparator = nullptr;
	
	struct AVLNode<T>* iterate(T data, int &direction, struct AVLNode<T>* cursor);
	int calculateNodeHeight (struct AVLNode<T>* node);
	int calculateBalanceFactor(struct AVLNode<T>* node);

	struct AVLNode<T>* rr_rotation(struct AVLNode<T>* node);
	struct AVLNode<T>* rl_rotation(struct AVLNode<T>* node);
	struct AVLNode<T>*	ll_rotation(struct AVLNode<T>* node);
	struct AVLNode<T>* lr_rotation(struct AVLNode<T>* node);

	struct AVLNode<T>* inpre(struct AVLNode<T>* node);
	struct AVLNode<T>* insuc(struct AVLNode<T>* node);

	struct AVLNode<T>* removeNode(T data, struct AVLNode<T>* cursor);
public:
	AVL(T_COMPARATOR comparator);
	void insert(T data);
	void remove(T data);
	struct AVLNode<T>* search (T data);
};