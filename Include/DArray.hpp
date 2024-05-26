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
	T& GetData(int index);
	int NumUsed();
	int PutData(T const& value);
	int PutData(T const& value, int index);
	void RemoveData(int index);
	void SetSize(int newSize);
	void SetStepSize(int newStepSize);
	bool ValidIndex(int index);

	int Size()
	{
		return size;
	}

	T& operator[](int index)
	{
		assert(index >= 0 && index < size);

		if (!dataValid[index])
			std::cout << "DArray error : DArray::[] called, referenced unused data.  (Index = " << index << ")\n";

		return this->data[index];
	}
};

#include "DArray.tpp"
