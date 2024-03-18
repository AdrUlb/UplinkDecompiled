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
	void PutData(const T& value);
	void PutDataAtEnd(const T& value);
	void RemoveData(int index);
	void Empty();
};

#include "LList.tpp"
