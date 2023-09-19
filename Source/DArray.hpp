#pragma once

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
private:
	~DArray();
	void Empty();
	T GetData(int index);
	int NumUsed();
public:
	int PutData(T* valuePtr);
private:
	void PutData(T* valuePtr, int index);
	void RemoveData(int index);
	void SetSize(int size);
	void SetStepSize(int stepSize);
	int Size();
	bool ValidIndex(int index);
};
