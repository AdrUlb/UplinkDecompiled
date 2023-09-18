#pragma once

#include "OptionChange.hpp"

template<typename T>
class LListItem
{
	T Value;
	LListItem<T>* Next = nullptr;
	LListItem<T>* Prev = nullptr;
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

template class LListItem<OptionChange*>;
