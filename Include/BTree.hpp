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

#include "BTree.tpp"
