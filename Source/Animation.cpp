#include <Animation.hpp>

Animation::Animation()
{
	buttonName = nullptr;
	button = nullptr;
	targetCaption = nullptr;
}

Animation::~Animation()
{
	if (buttonName != nullptr)
		delete[] buttonName;

	if (targetCaption != nullptr)
		delete[] targetCaption;
}
