#include <Probability.hpp>

Probability::Probability()
{
	int val = 0;
	_values.PutData(val);
}

Probability::~Probability()
{
	_values.Empty();
}

void Probability::InputProbability(int index, int value)
{
	if (index >= _values.Size())
		_values.SetSize(index + 1);
	_values.PutData(value, index);
}

bool Probability::Validate()
{
	int total = 0;

	for (auto i = 0; i < _values.Size(); i++)
	{
		if (!_values.ValidIndex(i))
			return false;

		const auto value = _values.GetData(i);

		if (value > 100)
			return false;

		total += value;
	}

	return total == 100;
}
