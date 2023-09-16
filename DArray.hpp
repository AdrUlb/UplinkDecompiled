#pragma once

template<typename T>
class DArray
{
private:
	int stepSize;
	int arraySize;
	T* data;
	bool* usedSlotsMap;
};
