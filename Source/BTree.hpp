#pragma once

#include "Bungle.hpp"
#include "DArray.hpp"

template<typename T>
class BTree
{
	BTree<T>* left = nullptr;
	BTree<T>* right = nullptr;
public:
	char* Name = nullptr;
	T Value;

	BTree();
	BTree(char* name, T* valuePtr);
	~BTree();
	void AppendRight(BTree<T>* value);
	DArray<char*>* ConvertIndexToDArray();
	DArray<T>* ConvertToDArray();
	void Empty();
	void PutData(char* name, T* valuePtr);
	void RecursiveConvertIndexToDArray(DArray<char*>* arr, BTree<T>* item);
	void RecursiveConvertToDArray(DArray<T>* arr, BTree<T>* item);
	inline BTree<T>* Left()
	{
		return left;
	}

	inline BTree<T>* Right()
	{
		return right;
	}
};

template class BTree<LocalFileHeader*>;
