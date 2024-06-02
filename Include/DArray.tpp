#pragma once

#include "DArray.hpp"

template <class T> DArray<T>::DArray() : _grow(1), _size(0), _data(nullptr), _dataValid(nullptr) {}
template <class T> DArray<T>::~DArray()
{
	Empty();
}

template <class T> void DArray<T>::Empty()
{
	if (_data != nullptr)
		delete[] _data;

	if (_dataValid != nullptr)
		delete[] _dataValid;

	_data = nullptr;
	_dataValid = nullptr;
	_size = 0;
}

template <class T> T& DArray<T>::GetData(int index) const
{
	assert(index < _size && index >= 0);

	if (!_dataValid[index])
		std::cout << "DArray::GetData called, referenced unused data.  (Index = " << index << ")\n";

	return _data[index];
}

template <class T> int DArray<T>::NumUsed()
{
	int count = 0;

	for (int i = 0; i < _size; i++)
		if (_dataValid[i])
			count++;

	return count;
}

template <class T> int DArray<T>::PutData(T const& value)
{
	// Find first valid index by iterating through all indices until the end is reached or an invalid element is found
	int index = 0;
	while (index < _size && _dataValid[index])
		index++;

	// If no valid index was found resize the array
	if (index == _size)
		SetSize(_size + this->_grow);

	_data[index] = value;
	_dataValid[index] = true;
	return index;
}

template <class T> int DArray<T>::PutData(T const& value, int index)
{
	assert(index < _size && index >= 0);
	_data[index] = value;
	_dataValid[index] = true;
	return index;
}

template <class T> void DArray<T>::RemoveData(int index)
{
	assert(index < _size && index >= 0);

	if (!_dataValid[index])
		std::cout << "Warning : DArray::RemoveData called, referenced unused data.  (Index = " << index << ")\n";

	_dataValid[index] = false;
}

template <class T> void DArray<T>::SetSize(const int newSize)
{
	// If the new size is the old size do nothing
	if (newSize == _size)
		return;

	// If the new size is <= 0 delete all data
	if (newSize <= 0)
	{
		_size = 0;

		if (_data != nullptr)
			delete[] _data;

		if (_dataValid != nullptr)
			delete[] _dataValid;

		_data = nullptr;
		_dataValid = nullptr;
		return;
	}

	// Save old size, set new size
	const auto oldSize = _size;
	_size = newSize;

	// Create new data and valid arrays
	const auto newData = new T[_size];
	const auto newDataValid = new bool[_size];

	// Set data
	for (int i = 0; i < _size; i++)
	{
		// If the index was valid before resizing copy the old data
		if (i < oldSize)
		{
			newData[i] = _data[i];
			newDataValid[i] = _dataValid[i];
		}
		else // Otherwise set index as invalid
			newDataValid[i] = false;
	}

	if (_data != nullptr)
		delete[] _data;

	if (_dataValid != nullptr)
		delete[] _dataValid;

	_data = newData;
	_dataValid = newDataValid;
}

template <class T> void DArray<T>::SetStepSize(const int newStepSize)
{
	_grow = newStepSize;
}

template <class T> bool DArray<T>::ValidIndex(int index) const
{
	if (index >= _size || index < 0)
		return false;

	return _dataValid[index];
}

template <class T> void DArray<T>::Sort(ComparatorFunc func)
{

	if (_size <= 0)
		return;

	auto validElementCount = 0;
	auto nextValid = 0;

	for (auto i = 0; i < _size; i++)
	{
		// If the current data is valid
		if (_dataValid[i])
		{
			// increment the valid element count
			validElementCount++;

			// If i has reached the next valid item, move the next valid item forward
			if (nextValid == i)
				nextValid++;

			// Next iteration
			continue;
		}

		// There are no more valid items in the array
		if (nextValid >= _size)
			break;

		// Find the next valid item
		while (!_dataValid[nextValid])
		{
			nextValid++;
			if (nextValid >= _size)
				break;
		}

		// There are no more valid items in the array
		if (nextValid >= _size)
			continue;

		// Copy date from next valid element to this element, mark this element as valid and the previously valid element as invalid
		_data[i] = _data[nextValid];
		_dataValid[i] = true;
		_dataValid[nextValid] = false;

		// Increase valid element count and next valid
		validElementCount++;
		nextValid++;
	}

	if (validElementCount > 0)
		qsort(this->_data, validElementCount, sizeof(T), reinterpret_cast<int (*)(const void*, const void*)>(func));
}