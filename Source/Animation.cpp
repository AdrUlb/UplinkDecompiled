#include <Animation.hpp>

Animation::~Animation()
{
	if (ButtonName != nullptr)
		delete[] ButtonName;

	if (TargetCaption != nullptr)
		delete[] TargetCaption;
}
