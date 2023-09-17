#pragma once

#include "Bungle.hpp"

template<typename T>
class BTree
{
	BTree<T>* left = nullptr;
	BTree<T>* right = nullptr;
	char* name = nullptr;
public:
	T value;

	BTree();
	~BTree();
	BTree(char* name, T* valuePtr);
	void Empty();
	void PutData(char* name, T* valuePtr);
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
