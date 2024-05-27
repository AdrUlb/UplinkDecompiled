#pragma once

#include "BTree.hpp"

#include <cstring>

template <class T> BTree<T>::BTree() : left(nullptr), right(nullptr), name(nullptr), Data{0} {}

template <class T> BTree<T>::BTree(const char* name, const T& value) : left(nullptr), right(nullptr)
{
	this->name = new char[strlen(name) + 1];
	strcpy(this->name, name);
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
	if (left != nullptr)
		delete left;

	if (right != nullptr)
		delete right;

	if (name != nullptr)
		delete[] name;

	right = nullptr;
	left = nullptr;
	name = nullptr;
}

template <class T> BTree<T>* BTree<T>::Left()
{
	return left;
}

template <class T> BTree<T>* BTree<T>::Right()
{
	return right;
}

template <class T> BTree<T>* BTree<T>::LookupTree(const char* name)
{
	auto tree = this;

	while (true)
	{
		if (tree->name == nullptr)
			return nullptr;

		const auto cmp = strcmp(name, tree->name);

		if (cmp == 0)
			return tree;

		if (tree->left != nullptr && cmp < 0)
		{
			tree = tree->left;
			continue;
		}

		if (tree->right != nullptr && cmp > 0)
		{
			tree = tree->right;
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
		if (tree->name == nullptr)
			break;

		if (strcmp(name, tree->name) > 0)
		{
			if (tree->right == nullptr)
			{
				tree->right = new BTree<T>(name, value);
				return;
			}
			tree = tree->right;
			continue;
		}

		if (tree->left == nullptr)
		{
			tree->left = new BTree<T>(name, value);
			return;
		}
		tree = tree->left;
	}

	tree->name = new char[strlen(name) + 1];
	strcpy(tree->name, name);
	tree->Data = value;
}

template <class T> void BTree<T>::RemoveData(const char* name)
{
	assert(name != nullptr);

	auto tree = this;

	while (true)
	{
		const auto cmp = strcmp(name, tree->name);

		const auto leftNode = tree->Left();
		const auto rightNode = tree->Right();

		// If this tree is the one we want to remove
		if (cmp == 0)
		{
			delete[] tree->name;

			// If there is a node to the left
			if (leftNode != nullptr)
			{
				tree->name = new char[strlen(leftNode->name) + 1];
				strcpy(tree->name, leftNode->name);
				tree->Data = leftNode->Data;
				tree->right = leftNode->Right();
				tree->left = leftNode->Left();
				tree->AppendRight(rightNode);
				return;
			}

			// If there is a node to the right
			if (rightNode != nullptr)
			{
				tree->name = new char[strlen(rightNode->name) + 1];
				auto rax_31 = rightNode;
				strcpy(tree->name, rax_31->name);
				tree->Data = rightNode->Data;
				tree->left = rightNode->Left();
				tree->right = rightNode->Right();
				return;
			}

			tree->name = nullptr;
			return;
		}

		// If the tree we are looking for is to the left
		if (cmp < 0)
		{
			// There is no tree to the left
			if (leftNode == nullptr)
				break;

			// If this tree is the one we want to remove and there is no tree to the left or right
			if (strcmp(leftNode->name, name) == 0 && leftNode->Left() == nullptr && leftNode->Right() == nullptr)
			{
				delete[] leftNode->name;
				tree->left = 0;
				return;
			}

			tree = leftNode;
			return;
		}

		// If the tree we are looking for is to the left

		// There is no tree to the right
		if (rightNode == nullptr)
			break;

		// If this tree is the one we want to remove and there is no tree to the left or right
		if (strcmp(rightNode->name, name) == 0 && rightNode->Left() == nullptr && rightNode->Right() == nullptr)
		{
			delete[] rightNode->name;
			tree->right = nullptr;
			break;
		}
		tree = rightNode;
	}
}

template <class T> void BTree<T>::RecursiveConvertToDArray(DArray<T>* array, BTree<T>* tree)
{
	assert(array != nullptr);

	for (auto i = tree; i != nullptr; i = i->Right())
	{
		if (i->name != nullptr)
			array->PutData(i->Data);

		RecursiveConvertToDArray(array, i->Left());
	}
}

template <class T> void BTree<T>::RecursiveConvertIndexToDArray(DArray<char*>* array, BTree<T>* tree)
{
	assert(array != nullptr);

	for (auto i = tree; i != nullptr; i = i->Right())
	{
		if (i->name != nullptr)
			array->PutData(i->name);

		RecursiveConvertIndexToDArray(array, i->Left());
	}
}

template <class T> void BTree<T>::AppendRight(BTree<T>* value)
{
	auto tree = this;

	while (true)
	{
		if (tree->Right() == nullptr)
			break;

		tree = tree->Right();
	}

	tree->right = value;
}
