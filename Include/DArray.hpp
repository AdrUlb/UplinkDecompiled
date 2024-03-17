#pragma once

#include <cassert>
#include <cstdint>
#include <iostream>

template <class T> class DArray
{
	int32_t grow;
	int32_t size;
	T* data;
	bool* dataValid;

public:
	DArray();
	~DArray();

	void Empty();
	T GetData(int index);
	int NumUsed();
	void PutData(T& value);
	void PutData(T& value, int index);
	void RemoveData(int index);
	void SetSize(int newSize);
	void SetStepSize(int newGrow);
	int Size();
	bool ValidIndex(int index);
};

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

template <class T> T DArray<T>::GetData(int index)
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

template <class T> void DArray<T>::PutData(T& value)
{
	// Find first valid index by iterating through all indices until the end is reached or an invalid element is found
	int validIndex = 0;
	while (validIndex < size && dataValid[validIndex])
		validIndex++;

	// If no valid index was found resize the array
	if (validIndex == size)
		SetSize(size + this->grow);

	data[validIndex] = value;
	dataValid[validIndex] = true;
}

template <class T> void DArray<T>::PutData(T& value, int index)
{
	assert(index < size && index >= 0);
	data[index] = value;
	dataValid[index] = true;
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
