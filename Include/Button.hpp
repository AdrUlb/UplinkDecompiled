#pragma once

#include <Image.hpp>

class Button;

typedef void (*ButtonDrawFunc)(Button*, bool, bool);
typedef void (*ButtonMouseDownFunc)(Button*);
typedef void (*ButtonMouseUpFunc)(Button*);
typedef void (*ButtonMouseMoveFunc)(Button*);

class Button
{
public:
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
	void SetCaption(const char* value);
	void SetStandardImage(Image* value);
	void RegisterDrawFunction(ButtonDrawFunc func);
	void RegisterMouseUpFunction(ButtonMouseUpFunc func);
	void RegisterMouseDownFunction(ButtonMouseDownFunc func);
	void RegisterMouseMoveFunction(ButtonMouseMoveFunc func);
	void SetTooltip(const char* value);
	void Draw(bool highlighted, bool clicked);
};
