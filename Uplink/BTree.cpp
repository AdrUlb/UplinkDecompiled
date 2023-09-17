#include "BTree.hpp"

#include <cstring>

template<typename T>
BTree<T>::BTree()
{
	left = nullptr;
	right = nullptr;
	name = nullptr;
	value = nullptr;
	return;
}

template<typename T>
BTree<T>::BTree(char* name, T* valuePtr)
{
	left = nullptr;
	right = nullptr;
	this->name = new char[strlen(name) + 1];
	strcpy(this->name, name);
	value = *valuePtr;
}

template<typename T>
BTree<T>::~BTree()
{
	Empty();
}

template<typename T>
void BTree<T>::Empty()
{
	if (left)
		delete left;

	if (right)
		delete right;

	if (name)
		delete[] name;

	right = nullptr;
	left = nullptr;
	name = nullptr;
}

template<typename T>
void BTree<T>::PutData(char* name, T* valuePtr)
{
	auto node = this;

	while (true)
	{
		if (node->name == (char*)0x0)
		{
			node->name = new char[strlen(name) + 1];
			strcpy(node->name, name);
			node->value = *valuePtr;
			return;
		}
		if (strcmp(name, node->name) < 1)
		{
			if (!node->left)
			{
				node->left = new BTree<T>(name, valuePtr);
				return;
			}
			node = node->left;
		}
		else
		{
			if (!node->right)
			{
				node->right = new BTree<T>(name, valuePtr);
				return;
			}
			node = node->right;
		}
	}
}
