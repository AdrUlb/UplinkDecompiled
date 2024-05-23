#include <Button.hpp>

void Button::SetStandardImage(Image* value)
{
	if (ImageNormal != nullptr)
		delete ImageNormal;
	ImageNormal = value;
}

void Button::RegisterDrawFunction(ButtonDrawFunc func)
{
	DrawFunc = func;
}

void Button::RegisterMouseUpFunction(ButtonMouseUpFunc func)
{
	MouseUpFunc = func;
}

void Button::RegisterMouseDownFunction(ButtonMouseDownFunc func)
{
	MouseDownFunc = func;
}

void Button::RegisterMouseMoveFunction(ButtonMouseMoveFunc func)
{
	MouseMoveFunc = func;
}
