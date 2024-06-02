#pragma once

#include <cstdio>

template <class T> struct LListItem
{
	T _value;
	LListItem<T>* _next = nullptr;
	LListItem<T>* _prev = nullptr;
};

template <class T> class LList
{
	LListItem<T>* _first;
	LListItem<T>* _last;
	LListItem<T>* _cachedItem;
	int _cachedIndex;
	int _length;

public:
	LList();
	~LList();

	T GetData(int index);
	void PutData(const T& value);
	void PutDataAtEnd(const T& value);
	void PutDataAtStart(const T& value);
	void RemoveData(int index);
	bool ValidIndex(int index);
	void Empty();
	void PutDataAtIndex(const T& value, int index);

	int Size()
	{
		return _length;
	}

	T operator[](int index)
	{
		return GetData(index);
	}
};

#include "LList.tpp"
