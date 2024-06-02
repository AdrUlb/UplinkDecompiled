#pragma once

#include "LList.hpp"

#include <Util.hpp>

template <class T> LList<T>::LList() : _first(nullptr), _last(nullptr), _cachedItem(nullptr), _cachedIndex(-1), _length(0) {}

template <class T> LList<T>::~LList()
{
	Empty();
}

template <class T> T LList<T>::GetData(int index)
{
	// If a cached item exists
	if (_cachedItem != nullptr && _cachedIndex != -1)
	{
		// If the requested item immediately follows the cached item
		if (index == _cachedIndex + 1)
		{
			// Update the cached item
			_cachedIndex++;
			_cachedItem = _cachedItem->_next;

			if (_cachedItem == nullptr)
				return 0;

			return _cachedItem->_value;
		}

		// If the requested item is the cached item return it
		if (index == _cachedIndex)
			return _cachedItem->_value;
	}

	// Get first item, return 0 if it is null
	auto item = _first;
	if (item == nullptr)
		return 0;

	// Find the requested index
	for (int i = 0; i < index; i++)
	{
		item = item->_next;
		if (item == nullptr)
			return 0;
	}

	// Set new cached item and return value
	_cachedIndex = index;
	_cachedItem = item;
	return item->_value;
}

template <class T> void LList<T>::PutData(const T& value)
{
	PutDataAtEnd(value);
}

template <class T> void LList<T>::PutDataAtEnd(const T& value)
{
	// Create the new item to be inserted at the end of the list
	const auto item = new LListItem<T>();
	item->_prev = _last; // The last item of the list becomes previous to this new last item
	item->_next = nullptr; // This is the last item, there is no next item yet
	item->_value = value;

	if (_last == nullptr) // If there was no last item the list was empty, this new item is now the first item
	{
		_first = item;
	}
	else // Otherwise, if there was a last item, set its next property
	{
		_last->_next = item;
	}

	// This is now the last item of the list
	_last = item;

	// Invalidate cached item
	_cachedIndex = -1;
	_cachedItem = nullptr;

	// Update length
	_length++;
}

template <class T> void LList<T>::PutDataAtStart(const T& value)
{
	const auto item = new LListItem<T>();
	item->_prev = nullptr;
	item->_next = _first;
	item->_value = value;

	if (_last == nullptr) // If the list is empty
	{
		_last = item; // This item is also the last item now
	}
	else // If the list is NOT empty
	{
		_first->_prev = item; // This item becomes previous to the prior first
	}

	// This is now the new first item
	_first = item;

	// Invalidate cached item
	_cachedIndex = -1;
	_cachedItem = nullptr;

	// Update length
	_length++;
}

template <class T> void LList<T>::RemoveData(int index)
{
	// Get first item, return if it is null
	auto item = _first;
	if (item == nullptr)
		return;

	// Find the requested index
	for (int i = 0; i < index; i++)
	{
		item = item->_next;
		if (item == nullptr)
			return;
	}

	// If the item was the first item (had no previous item) make the next item the first item
	if (item->_prev == nullptr)
		_first = item->_next;

	// If the item was the last item (had no next item) make the item the last item
	if (item->_next == nullptr)
		_last = item->_prev;

	// If the item was not the first item (had a previous item) make the previous item's next item the next item
	if (item->_prev != nullptr)
		item->_prev->_next = item->_next;

	// If the item was not the last item (had a next item) make the next item's previous item the previous item
	if (item->_next != nullptr)
		item->_next->_prev = item->_prev;

	_length--;

	delete item;

	// Invalidate cached item
	_cachedIndex = -1;
	_cachedItem = nullptr;
}

template <class T> bool LList<T>::ValidIndex(int index)
{
	return index >= 0 && index < _length;
}

template <class T> void LList<T>::Empty()
{
	auto item = this->_first;
	while (item != nullptr)
	{
		const auto next = item->_next;
		delete item;
		item = next;
	}

	this->_first = nullptr;
	this->_last = nullptr;
	this->_length = 0;
	this->_cachedItem = nullptr;
	this->_cachedIndex = -1;
}

template <class T> void LList<T>::PutDataAtIndex(const T& value, int index)
{
	if (index == 0)
	{
		PutDataAtStart(value);
		return;
	}

	if (index == _length)
	{
		PutDataAtEnd(value);
		return;
	}

	const auto item = new LListItem<T>();
	item->_value = value;

	auto prev = _first;
	for (int i = 0; i < index - 1; i++)
	{
		if (prev == nullptr)
			return;

		prev = prev->_next;
	}

	if (prev == nullptr)
		return;

	item->_prev = prev;
	item->_next = prev->_next;

	if (prev->_next != nullptr)
		prev->_next->_prev = item;

	prev->_next = item;

	_length++;
	_cachedItem = nullptr;
	_cachedIndex = -1;
}
