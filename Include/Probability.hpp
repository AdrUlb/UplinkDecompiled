#pragma once

#include <DArray.hpp>

class Probability
{
	DArray<int> _values;

public:
	Probability();
	virtual ~Probability();

	void InputProbability(int index, int value);
	bool Validate();
};
