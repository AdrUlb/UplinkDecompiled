#pragma once

#include <DArray.hpp>
#include <UplinkObject.hpp>

template <class T> class BTree
{
	struct BTree<T>* left;
	struct BTree<T>* right;
	char* name;

public:
	T data;

	BTree();
	BTree(const char* name, T& value);
	~BTree();

	BTree<T>* Left();
	BTree<T>* Right();

	BTree<T>* LookupTree(const char* name);
	T GetData(const char* name);
	void PutData(const char* name, T& value);

	void Empty();

	DArray<T>* ConvertToDArray();
	DArray<char*>* ConvertIndexToDArray();

	static void RecursiveConvertToDArray(DArray<T>* array, BTree<T>* tree);
	static void RecursiveConvertIndexToDArray(DArray<char*>* array, BTree<T>* tree);
};

void PrintBTree(BTree<UplinkObject*>* tree);
void PrintBTree(BTree<char*>* tree);

#include "BTree.tpp"
