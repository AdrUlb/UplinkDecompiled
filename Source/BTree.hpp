#pragma once

#include <cstring>
#include <cassert>
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


template<typename T>
BTree<T>::BTree()
{
	left = nullptr;
	right = nullptr;
	Name = nullptr;
	Value = nullptr;
	return;
}

template<typename T>
BTree<T>::BTree(char* name, T* valuePtr)
{
	left = nullptr;
	right = nullptr;
	this->Name = new char[strlen(name) + 1];
	strcpy(this->Name, name);
	Value = *valuePtr;
}

template<typename T>
BTree<T>::~BTree()
{
	Empty();
}

template<typename T>
void BTree<T>::AppendRight(BTree<T>* value)
{
	auto node = this;

	while (true)
	{
		if (!Right())
			break;

		node = Right();
	}

	node->right = value;
}

template<typename T>
DArray<char*>* BTree<T>::ConvertIndexToDArray()
{
	auto arr = new DArray<char*>();
	RecursiveConvertIndexToDArray(arr, this);
	return arr;
}

template<typename T>
DArray<T>* BTree<T>::ConvertToDArray()
{
	auto arr = new DArray<T>();
	RecursiveConvertToDArray(arr, this);
	return arr;
}

template<typename T>
void BTree<T>::Empty()
{
	if (left)
		delete left;

	if (right)
		delete right;

	if (Name)
		delete[] Name;

	right = nullptr;
	left = nullptr;
	Name = nullptr;
}

template<typename T>
void BTree<T>::PutData(char* name, T* valuePtr)
{
	auto node = this;

	while (true)
	{
		if (!node->Name)
		{
			node->Name = new char[strlen(name) + 1];
			strcpy(node->Name, name);
			node->Value = *valuePtr;
			return;
		}
		
		if (strcmp(name, node->Name) < 1)
		{
			if (!node->left)
			{
				node->left = new BTree<T>(name, valuePtr);
				return;
			}
			node = node->left;
		}
		else
		{
			if (!node->right)
			{
				node->right = new BTree<T>(name, valuePtr);
				return;
			}
			node = node->right;
		}
	}
}

template<typename T>
void BTree<T>::RecursiveConvertIndexToDArray(DArray<char*>* arr, BTree<T>* item)
{
	assert(arr);

	while (item)
	{
		if (item->Name)
			arr->PutData(&item->Name);

		RecursiveConvertIndexToDArray(arr, item->Left());

		item = item->Right();
	}
}

template<typename T>
void BTree<T>::RecursiveConvertToDArray(DArray<T>* arr, BTree<T>* item)
{
	assert(arr);

	while (item)
	{
		if (item->Name)
			arr->PutData(&item->Value);
		
		RecursiveConvertToDArray(arr, item->Left());

		item = item->Right();
	}
}
