#include <Probability.hpp>

Probability::Probability()
{
	int val = 0;
	values.PutData(val);
}

Probability::~Probability()
{
	values.Empty();
}

void Probability::InputProbability(int index, int value)
{
	if (index >= values.Size())
		values.SetSize(index + 1);
	values.PutData(value, index);
}

bool Probability::Validate()
{
	int total = 0;

	for (auto i = 0; i < values.Size(); i++)
	{
		if (!values.ValidIndex(i))
			return false;

		const auto value = values.GetData(i);

		if (value > 100)
			return false;

		total += value;
	}

	return total == 100;
}
