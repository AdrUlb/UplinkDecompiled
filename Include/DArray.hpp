#pragma once

#include <cassert>
#include <cstdint>
#include <iostream>

template <class T> class DArray
{
	int grow;
	int size;
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
	void SetStepSize(int newStepSize);
	bool ValidIndex(int index);
	
	inline int Size()
	{
		return size;
	}
};

#include "DArray.tpp"
