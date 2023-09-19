#include "DArray.hpp"

#include <cassert>
#include <iostream>

template<typename T>
DArray<T>::DArray()
{

}

template<typename T>
DArray<T>::~DArray()
{
	Empty();
}

template<typename T>
void DArray<T>::Empty()
{
	if (data)
		delete[] data;

	if (used)
		delete[] used;

	data = nullptr;
	used = nullptr;
	arraySize = 0;
}

template<typename T>
T DArray<T>::GetData(int index)
{
	assert(index >= 0 && index < arraySize);

	if (used[index] == false)
		std::cout << "DArray::GetData called, referenced unused data.  (Index = " << index << ")\n";

	return data[index];
}

template<typename T>
int DArray<T>::NumUsed()
{
	auto num = 0;

	for (auto i = 0; i < arraySize; i++)
		if (used[i] == true)
			num++;

	return num;
}

template<typename T>
int DArray<T>::PutData(T* valuePtr)
{
	for (auto i = 0; i < arraySize; i++)
	{
		if (!used[i])
		{
			data[i] = *valuePtr;
			used[i] = true;
			return i;
		}
	}

	SetSize(arraySize + stepSize);
	data[arraySize] = *valuePtr;
	used[arraySize] = true;
	return arraySize;
}

template<typename T>
void DArray<T>::PutData(T* valuePtr, int index)
{
	assert(index >= 0 && index < arraySize);

	data[index] = *valuePtr;
	used[index] = true;
}

template<typename T>
void DArray<T>::RemoveData(int index)
{
	assert(index >= 0 && index < arraySize);

	if (used[index] == false)
		std::cout << "Warning : DArray::RemoveData called, referenced unused data.  (Index  = " << index << ")\n";

	used[index] = false;
}

template<typename T>
void DArray<T>::SetSize(int size)
{
	if (size == 0)
	{
		arraySize = 0;

		if (data)
			delete[] data;

		if (used)
			delete[] used;

		data = nullptr;
		used = nullptr;
		return;
	}

	if (size == arraySize)
		return;

	auto newData = new T[size];
	auto newUsed = new bool[size];

	auto oldArraySize = arraySize;

	if (size < arraySize)
	{
		arraySize = size;

		for (auto i = 0; i < arraySize; i++)
		{
			newData[i] = data[i];
			newUsed[i] = used[i];
		}

		if (data)
			delete[] data;

		if (used)
			delete[] used;

		data = newData;
		used = newUsed;
		return;
	}

	arraySize = size;

	for (auto i = 0; i < oldArraySize; i++)
	{
		newData[i] = data[i];
		newUsed[i] = used[i];
	}

	for (auto i = oldArraySize; i < arraySize; i++)
	{
		newUsed[i] = false;
	}

	if (data)
		delete[] data;

	if (used)
		delete[] used;

	used = newUsed;
	data = newData;
}

template<typename T>
void DArray<T>::SetStepSize(int stepSize)
{
	this->stepSize = stepSize;
}

template<typename T>
int DArray<T>::Size()
{
	return arraySize;
}

template<typename T>
bool DArray<T>::ValidIndex(int index)
{
	if (index >= 0 && index < arraySize)
		return used[index];
	
	return false;
}
