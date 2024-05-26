#pragma once

#include "LList.hpp"

#include <Util.hpp>

template <class T> LListItem<T>::LListItem() : next(nullptr), prev(nullptr) {}

template <class T> LList<T>::LList() : first(nullptr), last(nullptr), cachedItem(nullptr), cachedIndex(-1), length(0) {}

template <class T> LList<T>::~LList()
{
	Empty();
}

template <class T> T LList<T>::GetData(int index)
{
	// If a cached item exists
	if (cachedItem != nullptr && cachedIndex != -1)
	{
		// If the requested item immediately follows the cached item
		if (index == cachedIndex + 1)
		{
			// Update the cached item
			cachedIndex++;
			cachedItem = cachedItem->next;

			if (cachedItem == nullptr)
				return 0;

			return cachedItem->value;
		}

		// If the requested item is the cached item return it
		if (index == cachedIndex)
			return cachedItem->value;
	}

	// Get first item, return 0 if it is null
	auto item = first;
	if (item == nullptr)
		return 0;

	// Find the requested index
	for (int i = 0; i < index; i++)
	{
		item = item->next;
		if (item == nullptr)
			return 0;
	}

	// Set new cached item and return value
	cachedIndex = index;
	cachedItem = item;
	return item->value;
}

template <class T> void LList<T>::PutData(const T& value)
{
	PutDataAtEnd(value);
}

template <class T> void LList<T>::PutDataAtEnd(const T& value)
{
	// Create the new item to be inserted at the end of the list
	const auto item = new LListItem<T>();
	item->prev = last; // The last item of the list becomes previous to this new last item
	item->next = nullptr; // This is the last item, there is no next item yet
	item->value = value;

	if (last == nullptr) // If there was no last item the list was empty, this new item is now the first item
	{
		first = item;
	}
	else // Otherwise, if there was a last item, set its next property
	{
		last->next = item;
	}

	// This is now the last item of the list
	last = item;

	// Invalidate cached item
	cachedIndex = -1;
	cachedItem = nullptr;

	// Update length
	length++;
}

template <class T> void LList<T>::PutDataAtStart(const T& value)
{
	const auto item = new LListItem<T>();
	item->prev = nullptr;
	item->next = first;
	item->value = value;

	if (last == nullptr) // If the list is empty
	{
		last = item; // This item is also the last item now
	}
	else // If the list is NOT empty
	{
		first->prev = item; // This item becomes previous to the prior first
	}

	// This is now the new first item
	first = item;

	// Invalidate cached item
	cachedIndex = -1;
	cachedItem = nullptr;

	// Update length
	length++;
}

template <class T> void LList<T>::RemoveData(int index)
{
	// Get first item, return if it is null
	auto item = first;
	if (item == nullptr)
		return;

	// Find the requested index
	for (int i = 0; i < index; i++)
	{
		item = item->next;
		if (item == nullptr)
			return;
	}

	// If the item was the first item (had no previous item) make the next item the first item
	if (item->prev == nullptr)
		first = item->next;

	// If the item was the last item (had no next item) make the item the last item
	if (item->next == nullptr)
		last = item->prev;

	// If the item was not the first item (had a previous item) make the previous item's next item the next item
	if (item->prev != nullptr)
		item->prev->next = item->next;

	// If the item was not the last item (had a next item) make the next item's previous item the previous item
	if (item->next != nullptr)
		item->next->prev = item->prev;

	length--;

	delete item;

	// Invalidate cached item
	cachedIndex = -1;
	cachedItem = nullptr;
}

template <class T> bool LList<T>::ValidIndex(int index)
{
	return index >= 0 && index < length;
}

template <class T> void LList<T>::Empty()
{
	auto item = this->first;
	while (item != nullptr)
	{
		const auto next = item->next;
		delete item;
		item = next;
	}

	this->first = nullptr;
	this->last = nullptr;
	this->length = 0;
	this->cachedItem = nullptr;
	this->cachedIndex = -1;
}

template <class T> bool LoadLList(LList<T>* list, FILE* file)
{
	UplinkAssert(list != nullptr);

	int itemCount;
	if (!FileReadData(&itemCount, 4, 1, file))
		return false;

	if (itemCount > 0x40000)
	{
		printf("Print Abort: %s ln %d : ", __FILE__, __LINE__);
		printf("WARNING: LoadLList, number of items appears to be wrong, size=%d\n", itemCount);
		return false;
	}

	for (auto i = 0; i < itemCount; i++)
	{
		char* str = nullptr;

		if (!LoadDynamicString(str, file))
			return false;

		list->PutData(str);
	}

	return true;
}
