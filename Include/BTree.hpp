#pragma once

#include <DArray.hpp>

template <class T> class BTree
{
	BTree<T>* _left;
	BTree<T>* _right;
	char* _name;

public:
	T Data;

	BTree();
	BTree(const char* name, const T& value);
	~BTree();

	BTree<T>* Left();
	BTree<T>* Right();

	BTree<T>* LookupTree(const char* name);
	T GetData(const char* name);
	void PutData(const char* name, const T& value);
	void RemoveData(const char* key);

	void Empty();

	DArray<T>* ConvertToDArray();
	DArray<char*>* ConvertIndexToDArray();

	void AppendRight(BTree<T>* tree);

	static void RecursiveConvertToDArray(DArray<T>* array, BTree<T>* tree);
	static void RecursiveConvertIndexToDArray(DArray<char*>* array, BTree<T>* tree);
};

#include "BTree.tpp"
