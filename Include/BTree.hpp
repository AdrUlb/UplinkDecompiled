#pragma once

#include <DArray.hpp>

template <class T> class BTree
{
	struct BTree<T>* left;
	struct BTree<T>* right;
	char* name;
	T value;

public:
	BTree();
	BTree(const char* name, T& value);
	~BTree();

	DArray<T>* ConvertToDArray();
	void Empty();
	BTree<T>* Left();
	BTree<T>* Right();
	BTree<T>* LookupTree(const char* name);
	void PutData(const char* name, T& value);
	static void RecursiveConvertToDArray(DArray<T>* array, BTree<T>* tree);
};

template <class T> BTree<T>::BTree() : left(nullptr), right(nullptr), name(nullptr), value{0} {}

template <class T> BTree<T>::BTree(const char* name, T& value) : left(nullptr), right(nullptr)
{
	this->name = new char[strlen(name + 1)];
	strcpy(this->name, name);
	this->value = value;
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
	tree->value = value;
}

template <class T> void BTree<T>::RecursiveConvertToDArray(struct DArray<T>* array, struct BTree<T>* tree)
{
	assert(array != nullptr);

	for (auto i = tree; i != nullptr; i = i->Right())
	{
		if (i->name != 0)
			array->PutData(i->value);

		RecursiveConvertToDArray(array, i->Left());
	}
}
