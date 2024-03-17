#pragma once

template <class T> struct LListItem
{
	T value;
	LListItem<T>* next;
	LListItem<T>* prev;

	LListItem();
};

template <class T> class LList
{
	LListItem<T>* first;
	LListItem<T>* last;
	LListItem<T>* cachedItem;
	int cachedIndex;
	int length;

public:
	LList();
	~LList();

	T GetData(int index);
	void PutData(T& value);
	void PutDataAtEnd(T& value);
	void RemoveData(int index);
	void Empty();
};

#include "LList.tpp"
