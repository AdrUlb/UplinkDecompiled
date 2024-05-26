#include <Button.hpp>

#include <cstring>

Button::Button(int x, int y, int width, int height, const char* caption, const char* name)
{
	Tooltip = nullptr;
	Name = nullptr;
	Caption = nullptr;
	SetProperties(x, y, width, height, caption, name);
	ImageNormal = nullptr;
	ImageHighlighted = nullptr;
	ImageClicked = nullptr;
	DrawFunc = nullptr;
	MouseDownFunc = nullptr;
	MouseUpFunc = nullptr;
	MouseMoveFunc = nullptr;
	SetTooltip(" ");
	field_30 = 0;
}

Button::~Button()
{
	if (Caption != nullptr)
		delete[] Caption;

	if (Tooltip != nullptr)
		delete[] Tooltip;

	if (Name != nullptr)
		delete[] Name;

	bool deleteHighlighted = ImageHighlighted != nullptr && ImageHighlighted != ImageNormal;
	bool deleteClicked = ImageClicked != nullptr && ImageClicked != ImageNormal && ImageClicked != ImageHighlighted;

	if (ImageNormal != nullptr)
		delete ImageNormal;

	if (ImageHighlighted != nullptr && deleteHighlighted)
		delete ImageHighlighted;

	if (ImageClicked != nullptr && deleteClicked)
		delete ImageClicked;
}

void Button::SetProperties(int x, int y, int width, int height, const char* caption, const char* name)
{
	X = x;
	Y = y;
	Width = width;
	Height = height;

	if (Name != nullptr)
		delete[] Name;

	Name = new char[strlen(name) + 1];
	strcpy(Name, name);
	SetCaption(caption);
}

void Button::SetCaption(const char* value)
{
	if (Caption != nullptr && strcmp(value, Caption) == 0)
		return;

	Dirty = true;

	if (Caption != nullptr)
		delete[] Caption;

	Caption = new char[strlen(value) + 1];
	strcpy(Caption, value);
}

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

void Button::SetTooltip(const char* value)
{
	if (Tooltip != nullptr)
		delete[] Tooltip;

	Tooltip = new char[strlen(value) + 1];
	strcpy(Tooltip, value);
}

void Button::Draw(bool highlighted, bool clicked)
{
	if (DrawFunc == nullptr)
		return;

	DrawFunc(this, highlighted, clicked);
}

void Button::SetImages(Image* imageNormal, Image* imageHighlighted, Image* imageClicked)
{
	if (ImageNormal != nullptr)
		delete ImageNormal;

	if (ImageHighlighted != nullptr)
		delete ImageHighlighted;

	if (ImageClicked != nullptr)
		delete ImageClicked;

	ImageNormal = imageNormal;
	ImageHighlighted = imageHighlighted;
	ImageClicked = imageClicked;
}
