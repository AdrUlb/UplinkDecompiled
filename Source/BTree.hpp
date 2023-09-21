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
	T Data;

	BTree();
	BTree(char* name, T* dataPtr);
	~BTree();
	void AppendRight(BTree<T>* value);
	DArray<char*>* ConvertIndexToDArray();
	DArray<T>* ConvertToDArray();
	void Empty();
	BTree<T>* LookupTree(const char* name);
	void PutData(char* name, T* dataPtr);
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
	Data = nullptr;
	return;
}

template<typename T>
BTree<T>::BTree(char* name, T* dataPtr)
{
	left = nullptr;
	right = nullptr;
	this->Name = new char[strlen(name) + 1];
	strcpy(this->Name, name);
	Data = *dataPtr;
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
BTree<T>* BTree<T>::LookupTree(const char* name)
{
	char* thisName;
	int iVar1;
	BTree<char*>* pBVar2;

	auto node = this;

	while (true)
	{
		if (!node->Name)
			return nullptr;

		iVar1 = strcmp(name, node->Name);
		if (iVar1 == 0)
			return node;

		else if (iVar1 < 0)
		{
			if (!node->left)
				return nullptr;

			node = node->left;
		}
		else
		{
			if (!node->right)
				return nullptr;

			node = node->right;
		}
	}
}

template<typename T>
void BTree<T>::PutData(char* name, T* dataPtr)
{
	auto node = this;

	while (true)
	{
		if (!node->Name)
		{
			node->Name = new char[strlen(name) + 1];
			strcpy(node->Name, name);
			node->Data = *dataPtr;
			return;
		}

		if (strcmp(name, node->Name) <= 0)
		{
			if (!node->left)
			{
				node->left = new BTree<T>(name, dataPtr);
				return;
			}
			node = node->left;
		}
		else
		{
			if (!node->right)
			{
				node->right = new BTree<T>(name, dataPtr);
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
			arr->PutData(&item->Data);

		RecursiveConvertToDArray(arr, item->Left());

		item = item->Right();
	}
}
