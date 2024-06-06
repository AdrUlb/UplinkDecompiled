#pragma once

#include "BTree.hpp"

#include <cstring>

template <class T> BTree<T>::BTree() : _left(nullptr), _right(nullptr), _name(nullptr), Data{0} {}

template <class T> BTree<T>::BTree(const char* name, const T& value) : _left(nullptr), _right(nullptr)
{
	this->_name = new char[strlen(name) + 1];
	strcpy(this->_name, name);
	this->Data = value;
}

template <class T> BTree<T>::~BTree()
{
	Empty();
}

template <class T> DArray<T>* BTree<T>::ConvertToDArray()
{
	const auto array = new DArray<T>();
	RecursiveConvertToDArray(array, this);
	return array;
}
template <class T> DArray<char*>* BTree<T>::ConvertIndexToDArray()
{
	const auto array = new DArray<char*>();
	RecursiveConvertIndexToDArray(array, this);
	return array;
}

template <class T> void BTree<T>::Empty()
{
	if (_left != nullptr)
		delete _left;

	if (_right != nullptr)
		delete _right;

	if (_name != nullptr)
		delete[] _name;

	_right = nullptr;
	_left = nullptr;
	_name = nullptr;
}

template <class T> BTree<T>* BTree<T>::Left()
{
	return _left;
}

template <class T> BTree<T>* BTree<T>::Right()
{
	return _right;
}

template <class T> BTree<T>* BTree<T>::LookupTree(const char* name)
{
	auto tree = this;

	while (true)
	{
		if (tree->_name == nullptr)
			return nullptr;

		const auto cmp = strcmp(name, tree->_name);

		if (cmp == 0)
			return tree;

		if (tree->_left != nullptr && cmp < 0)
		{
			tree = tree->_left;
			continue;
		}

		if (tree->_right != nullptr && cmp > 0)
		{
			tree = tree->_right;
			continue;
		}

		return nullptr;
	}
}

template <class T> T BTree<T>::GetData(const char* name)
{
	const auto tree = LookupTree(name);

	if (tree == nullptr)
		return nullptr;

	return tree->Data;
}

template <class T> void BTree<T>::PutData(const char* name, const T& value)
{
	auto tree = this;

	while (true)
	{
		if (tree->_name == nullptr)
			break;

		if (strcmp(name, tree->_name) > 0)
		{
			if (tree->_right == nullptr)
			{
				tree->_right = new BTree<T>(name, value);
				return;
			}
			tree = tree->_right;
			continue;
		}

		if (tree->_left == nullptr)
		{
			tree->_left = new BTree<T>(name, value);
			return;
		}
		tree = tree->_left;
	}

	tree->_name = new char[strlen(name) + 1];
	strcpy(tree->_name, name);
	tree->Data = value;
}

template <class T> void BTree<T>::RemoveData(const char* name)
{
	assert(name != nullptr);

	auto tree = this;

	while (true)
	{
		const auto cmp = strcmp(name, tree->_name);

		const auto leftNode = tree->_left;
		const auto rightNode = tree->_right;

		// If this tree is the one we want to remove
		if (cmp == 0)
		{
			delete[] tree->_name;

			// If there is a node to the left
			if (leftNode != nullptr)
			{
				tree->_name = leftNode->_name;
				tree->Data = leftNode->Data;
				tree->_right = leftNode->_right;
				tree->_left = leftNode->_left;
				tree->AppendRight(rightNode);
				return;
			}

			// If there is a node to the right
			if (rightNode != nullptr)
			{
				tree->_name = rightNode->_name;
				tree->Data = rightNode->Data;
				tree->_left = rightNode->_left;
				tree->_right = rightNode->_right;
				return;
			}

			tree->_name = nullptr;
			return;
		}

		// If the tree we are looking for is to the left
		if (cmp < 0)
		{
			// There is no tree to the left
			if (leftNode == nullptr)
				break;

			// If this tree is the one we want to remove and there is no tree to the left or right
			if (strcmp(leftNode->_name, name) == 0 && leftNode->_left == nullptr && leftNode->_right == nullptr)
			{
				delete[] leftNode->_name;
				tree->_left = 0;
				return;
			}

			tree = leftNode;
			return;
		}

		// If the tree we are looking for is to the right

		// There is no tree to the right
		if (rightNode == nullptr)
			break;

		// If this tree is the one we want to remove and there is no tree to the left or right
		if (strcmp(rightNode->_name, name) == 0 && rightNode->_left == nullptr && rightNode->_right == nullptr)
		{
			delete[] rightNode->_name;
			tree->_right = nullptr;
			break;
		}
		tree = rightNode;
	}
}

template <class T> void BTree<T>::RemoveData(const char* name, const T& value)
{
	assert(name != nullptr);

	struct BTree<char*>* tree = this;
	while (true)
	{
		const auto cmp = strcmp(name, tree->_name);

		const auto leftNode = tree->Left();
		const auto rightNode = tree->Right();

		// If this is the tree we are looking for
		if (cmp == 0)
		{
			// If this is also the correct value
			if (tree->Data == value)
			{
				// There is a tree to the left
				if (leftNode != nullptr)
				{
					tree->_name = leftNode->_name;
					tree->Data = leftNode->Data;
					tree->_right = leftNode->Right();
					tree->_left = leftNode->Left();
					tree->AppendRight(rightNode);
					return;
				}

				// There is a tree to the right
				if (rightNode != nullptr)
				{
					tree->_name = rightNode->_name;
					tree->Data = rightNode->Data;
					tree->_left = rightNode->Left();
					tree->_right = rightNode->Right();
					return;
				}

				tree->_name = nullptr;
				return;
			}
		}

		// The tree we are looking for is to the left
		if (cmp < 0)
		{
			// There is no tree to the left
			if (tree->Left() == nullptr)
				break;

			if (strcmp(tree->Left()->_name, name) == 0 && tree->Data == value && tree->Left()->Left() == 0 && tree->Left()->Right() == 0)
			{
				tree->_left = 0;
				break;
			}

			tree = tree->Left();
			continue;
		}

		// The tree we are looking for is to the right

		// There is no tree to the right
		if (tree->Right() == nullptr)
			break;

		if (strcmp(tree->Right()->_name, name) == 0 && tree->Data == value && tree->Right()->Left() == nullptr && tree->Right()->Right() == nullptr)
		{
			tree->_right = nullptr;
			break;
		}
		tree = tree->Right();
	}
}

template <class T> void BTree<T>::RecursiveConvertToDArray(DArray<T>* array, BTree<T>* tree)
{
	assert(array != nullptr);

	for (auto i = tree; i != nullptr; i = i->_right)
	{
		if (i->_name != nullptr)
			array->PutData(i->Data);

		RecursiveConvertToDArray(array, i->_left);
	}
}

template <class T> void BTree<T>::RecursiveConvertIndexToDArray(DArray<char*>* array, BTree<T>* tree)
{
	assert(array != nullptr);

	for (auto i = tree; i != nullptr; i = i->_right)
	{
		if (i->_name != nullptr)
			array->PutData(i->_name);

		RecursiveConvertIndexToDArray(array, i->_left);
	}
}

template <class T> void BTree<T>::AppendRight(BTree<T>* value)
{
	auto tree = this;

	while (true)
	{
		if (tree->_right == nullptr)
			break;

		tree = tree->_right;
	}

	tree->_right = value;
}
