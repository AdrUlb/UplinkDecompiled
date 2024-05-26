#pragma once

#include <DArray.hpp>
#include <UplinkObject.hpp>

template <class T> class BTree
{
	BTree<T>* left;
	BTree<T>* right;
	char* name;

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

bool LoadBTree(BTree<UplinkObject*>* tree, FILE* file);
void PrintBTree(BTree<UplinkObject*>* tree);
void PrintBTree(BTree<char*>* tree);
void SaveBTree(BTree<UplinkObject*>* tree, FILE* file);
void UpdateBTree(BTree<UplinkObject*>* tree);

#include "BTree.tpp"
