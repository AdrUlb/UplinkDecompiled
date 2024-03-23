#pragma once

#include "BTree.hpp"

#include <Util.hpp>
#include <cstring>

template <class T> BTree<T>::BTree() : left(nullptr), right(nullptr), name(nullptr), data{0} {}

template <class T> BTree<T>::BTree(const char* name, T& value) : left(nullptr), right(nullptr)
{
	this->name = new char[strlen(name) + 1];
	strcpy(this->name, name);
	this->data = value;
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

	return tree->data;
}

template <class T> void BTree<T>::PutData(char const* name, T& value)
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
	tree->data = value;
}

template <class T> void BTree<T>::RecursiveConvertToDArray(DArray<T>* array, BTree<T>* tree)
{
	assert(array != nullptr);

	for (auto i = tree; i != nullptr; i = i->Right())
	{
		if (i->name != nullptr)
			array->PutData(i->data);

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

template <class T> void DeleteBTreeData(BTree<T>* tree)
{
	UplinkAssert(tree != nullptr);

	const auto array = tree->ConvertToDArray();

	for (int i = 0; i < array->Size(); i++)
	{
		const auto data = array->GetData(i);
		if (array->ValidIndex(i) && data != nullptr)
		{
			if (data != nullptr)
				delete data;
		}
	}

	delete (array);
}
