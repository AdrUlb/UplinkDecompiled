#include <NumberGenerator.hpp>
#include <random>

int NumberGenerator::RandomNumber(int max)
{
	int ret = rand() * 4.65661287e-10f * max;
	if (ret < 0)
		ret = 0;

	if (ret >= max)
		ret = max - 1;

	return ret;
}
