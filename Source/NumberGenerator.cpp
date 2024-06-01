#include <NumberGenerator.hpp>
#include <random>

int NumberGenerator::RandomNumber(int max)
{
	int ret = static_cast<double>(rand()) / RAND_MAX * max;
	if (ret < 0)
		ret = 0;

	if (ret >= max)
		ret = max - 1;

	return ret;
}

int NumberGenerator::RandomNormalNumber(float mean, float deviation)
{
	auto a = 0.0f;

	for (auto i = 0; i < 12; i++)
		a += static_cast<double>(rand()) / RAND_MAX;

	a = (a - 6.0f) * (deviation / 3.0f) + mean;

	a = std::max(mean - deviation, a);
	a = std::min(mean + deviation, a);

	return a;
}

int NumberGenerator::ApplyVariance(int value, int deviation)
{
	return (RandomNormalNumber(0.0f, deviation) + 100.0f) / 100.0f * value;
}
