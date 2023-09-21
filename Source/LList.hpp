#pragma once

#include "OptionChange.hpp"

template<typename T>
class LListItem
{
	T value;
	LListItem<T>* next = nullptr;
	LListItem<T>* prev = nullptr;
};

template<typename T>
class LList
{
	LListItem<T>* first = nullptr;
	LListItem<T>* last = nullptr;
	LListItem<T>* cached = nullptr;
	int cachedIndex = -1;
	int size = 0;
};
