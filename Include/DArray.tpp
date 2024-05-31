#pragma once

#include "DArray.hpp"

template <class T> DArray<T>::DArray() : grow(1), size(0), data(nullptr), dataValid(nullptr) {}
template <class T> DArray<T>::~DArray()
{
	Empty();
}

template <class T> void DArray<T>::Empty()
{
	if (data != nullptr)
		delete[] data;

	if (dataValid != nullptr)
		delete[] dataValid;

	data = nullptr;
	dataValid = nullptr;
	size = 0;
}

template <class T> T& DArray<T>::GetData(int index) const
{
	assert(index < size && index >= 0);

	if (!dataValid[index])
		std::cout << "DArray::GetData called, referenced unused data.  (Index = " << index << ")\n";

	return data[index];
}

template <class T> int DArray<T>::NumUsed()
{
	int count = 0;

	for (int i = 0; i < size; i++)
		if (dataValid[i])
			count++;

	return count;
}

template <class T> int DArray<T>::PutData(T const& value)
{
	// Find first valid index by iterating through all indices until the end is reached or an invalid element is found
	int index = 0;
	while (index < size && dataValid[index])
		index++;

	// If no valid index was found resize the array
	if (index == size)
		SetSize(size + this->grow);

	data[index] = value;
	dataValid[index] = true;
	return index;
}

template <class T> int DArray<T>::PutData(T const& value, int index)
{
	assert(index < size && index >= 0);
	data[index] = value;
	dataValid[index] = true;
	return index;
}

template <class T> void DArray<T>::RemoveData(int index)
{
	assert(index < size && index >= 0);

	if (!dataValid[index])
		std::cout << "Warning : DArray::RemoveData called, referenced unused data.  (Index = " << index << ")\n";

	dataValid[index] = false;
}

template <class T> void DArray<T>::SetSize(const int newSize)
{
	// If the new size is the old size do nothing
	if (newSize == size)
		return;

	// If the new size is <= 0 delete all data
	if (newSize <= 0)
	{
		size = 0;

		if (data != nullptr)
			delete[] data;

		if (dataValid != nullptr)
			delete[] dataValid;

		data = nullptr;
		dataValid = nullptr;
		return;
	}

	// Save old size, set new size
	const auto oldSize = size;
	size = newSize;

	// Create new data and valid arrays
	const auto newData = new T[size];
	const auto newDataValid = new bool[size];

	// Set data
	for (int i = 0; i < size; i++)
	{
		// If the index was valid before resizing copy the old data
		if (i < oldSize)
		{
			newData[i] = data[i];
			newDataValid[i] = dataValid[i];
		}
		else // Otherwise set index as invalid
			newDataValid[i] = false;
	}

	if (data != nullptr)
		delete[] data;

	if (dataValid != nullptr)
		delete[] dataValid;

	data = newData;
	dataValid = newDataValid;
}

template <class T> void DArray<T>::SetStepSize(const int newStepSize)
{
	grow = newStepSize;
}

template <class T> bool DArray<T>::ValidIndex(int index) const
{
	if (index >= size || index < 0)
		return false;

	return dataValid[index];
}

template <class T> void DArray<T>::Sort(ComparatorFunc func)
{

	if (size <= 0)
		return;

	auto validElementCount = 0;
	auto nextValid = 0;

	for (auto i = 0; i < size; i++)
	{
		// If the current data is valid
		if (dataValid[i])
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
		if (nextValid >= size)
			break;

		// Find the next valid item
		while (!dataValid[nextValid])
		{
			nextValid++;
			if (nextValid >= size)
				break;
		}

		// There are no more valid items in the array
		if (nextValid >= size)
			continue;

		// Copy date from next valid element to this element, mark this element as valid and the previously valid element as invalid
		data[i] = data[nextValid];
		dataValid[i] = true;
		dataValid[nextValid] = false;

		// Increase valid element count and next valid
		validElementCount++;
		nextValid++;
	}

	if (validElementCount > 0)
		qsort(this->data, validElementCount, sizeof(T), reinterpret_cast<int (*)(const void*, const void*)>(func));
}