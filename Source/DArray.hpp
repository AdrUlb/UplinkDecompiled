#pragma once

#include <cassert>
#include <iostream>

template<typename T>
class DArray
{
private:
	int stepSize = 1;
	int arraySize = 0;
	T* data = nullptr;
	bool* used = nullptr;

public:
	DArray();
	~DArray();
private:
	void Empty();
public:
	T GetData(int index);
private:
	int NumUsed();
public:
	int PutData(T* valuePtr);
private:
	void PutData(T* valuePtr, int index);
	void RemoveData(int index);
	void SetSize(int size);
	void SetStepSize(int stepSize);
public:
	int Size();
	bool ValidIndex(int index);
};

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

	auto prevArraySize = arraySize;
	SetSize(arraySize + stepSize);
	data[prevArraySize] = *valuePtr;
	used[prevArraySize] = true;
	return prevArraySize;
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
		Empty();
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
