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

#include "DArray.tpp"
