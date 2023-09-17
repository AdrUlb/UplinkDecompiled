#pragma once

template<typename T>
class DArray
{
private:
	int stepSize;
	int arraySize;
	T* data;
	bool* usedSlotsMap;

	DArray();
	~DArray();
	void Empty();
	T GetData(int index);
	int NumUsed();
	int PutData(char* value);
	void RemoveData(int index);
	void SetSize(int size);
	void SetStepSize(int stepSize);
	int Size();
	bool ValidIndex(int index);
};
