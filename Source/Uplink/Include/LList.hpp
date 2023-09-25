#pragma once

#include <Button.hpp>

template<typename T>
class LListItem
{
public:
	T value;
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

public:
	void Empty();
	T GetData(int index);
	void RemoveData(int index);
	bool ValidIndex(int index);
};

template<typename T>
void LList<T>::Empty()
{
	auto item = first;

	while (item != nullptr)
	{
		const auto next = item->Next;
		delete item;
		item = next;
	}

	first = nullptr;
	last = nullptr;
	size = 0;
	cached = nullptr;
	cachedIndex = -1;
	return;
}

template<typename T>
T LList<T>::GetData(int index)
{
	if (cached && cachedIndex != -1)
	{
		if (index == cachedIndex + 1)
		{
			cachedIndex++;

			cached = cached->Next;
			if (!cached)
				return nullptr;

			return cached->value;
		}

		if (index == cachedIndex)
			return cached->value;
	}

	auto item = this->first;
	for (auto i = 0; i < index; i++)
	{
		if (!item)
			return nullptr;

		item = item->Next;
	}

	this->cached = item;

	if (!item)
	{
		this->cachedIndex = -1;
		return nullptr;
	}

	this->cachedIndex = index;

	return item->value;
}

template<typename T>
bool LList<T>::ValidIndex(int index)
{
	return index >= 0 && index < size;
}
