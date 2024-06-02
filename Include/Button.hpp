#pragma once

#include <Image.hpp>

class Button;

typedef void (*ButtonDrawFunc)(Button* button, bool highlighted, bool clicked);
typedef void (*ButtonMouseDownFunc)(Button* button);
typedef void (*ButtonMouseUpFunc)(Button* button);
typedef void (*ButtonMouseMoveFunc)(Button* button);

struct Button
{
	int X;
	int Y;
	int Width;
	int Height;
	char* Caption;
	char* Name;
	char* Tooltip;
	int field_30;
	bool Dirty;
	Image* ImageNormal;
	Image* ImageHighlighted;
	Image* ImageClicked;
	ButtonDrawFunc DrawFunc;
	ButtonMouseDownFunc MouseDownFunc;
	ButtonMouseUpFunc MouseUpFunc;
	ButtonMouseMoveFunc MouseMoveFunc;

	Button(int x, int y, int width, int height, const char* caption, const char* name);
	virtual ~Button();
	void SetProperties(int x, int y, int width, int height, const char* caption, const char* name);
	void SetCaption(const char* caption);
	void SetStandardImage(Image* value);
	void RegisterDrawFunction(ButtonDrawFunc func);
	void RegisterMouseUpFunction(ButtonMouseUpFunc func);
	void RegisterMouseDownFunction(ButtonMouseDownFunc func);
	void RegisterMouseMoveFunction(ButtonMouseMoveFunc func);
	void SetTooltip(const char* tooltip);
	void Draw(bool highlighted, bool clicked);
	void SetImages(Image* imageNormal, Image* imageHighlighted, Image* imageClicked);
	void MouseMove();
	void MouseDown();
	void MouseUp();
};
