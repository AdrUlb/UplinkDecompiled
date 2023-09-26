#pragma once

#include <Image.hpp>

class Button;

typedef void(*ButtonDrawFunc)(Button*, bool, bool);
typedef void(*ButtonMouseUpFunc)(Button*);
typedef void(*ButtonMouseDownFunc)(Button*);
typedef void(*ButtonMouseMoveFunc)(Button*);

class Button
{
	int x;
	int y;
	int width;
	int height;
	char* caption;
public:
	char* Name;
private:
	char* tooltip;
	int style;
public:
	bool Dirty;
private:
	Image* image1;
	Image* image2;
	Image* image3;
	ButtonDrawFunc drawFunc;
	ButtonMouseDownFunc mouseDownFunc;
	ButtonMouseUpFunc mouseUpFunc;
	ButtonMouseMoveFunc mouseMoveFunc;

	Button();
	Button(int x, int y, int width, int height, const char* caption, const char* name);
	virtual ~Button();
};
