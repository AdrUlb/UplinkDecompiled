#pragma once

#include <Image.hpp>

struct Button;

typedef void (*ButtonDrawFunc)(struct Button*, bool, bool);
typedef void (*ButtonMouseDownFunc)(struct Button*);
typedef void (*ButtonMouseUpFunc)(struct Button*);
typedef void (*ButtonMouseMoveFunc)(struct Button*);

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

	Button();
	virtual ~Button();
	void SetStandardImage(Image* value);
	void RegisterDrawFunction(ButtonDrawFunc func);
	void RegisterMouseUpFunction(ButtonMouseUpFunc func);
	void RegisterMouseDownFunction(ButtonMouseDownFunc func);
	void RegisterMouseMoveFunction(ButtonMouseMoveFunc func);
};
