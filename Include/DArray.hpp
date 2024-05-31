#pragma once

#include <cassert>
#include <cstdint>
#include <cstdlib>
#include <iostream>

template <class T> class DArray
{
	int grow;
	int size;
	T* data;
	bool* dataValid;

public:
	typedef int (*ComparatorFunc)(T* a, T* b);

	DArray();
	~DArray();

	void Empty();
	T& GetData(int index) const;
	int NumUsed();
	int PutData(T const& value);
	int PutData(T const& value, int index);
	void RemoveData(int index);
	void SetSize(int newSize);
	void SetStepSize(int newStepSize);
	bool ValidIndex(int index) const;
	void Sort(ComparatorFunc func);

	int Size() const
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
