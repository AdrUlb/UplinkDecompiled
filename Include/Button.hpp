#pragma once

#include <Image.hpp>

class Button;

typedef void (*ButtonDrawFunc)(struct Button*, bool, bool);
typedef void (*ButtonMouseDownFunc)(struct Button*);
typedef void (*ButtonMouseUpFunc)(struct Button*);
typedef void (*ButtonMouseMoveFunc)(struct Button*);

class Button
{
	int x;
	int y;
	int width;
	int height;
	char* caption;
	char* name;
	char* tooltip;
	int field_30;
	bool dirty;
	struct Image* image1;
	struct Image* image2;
	struct Image* image3;
	ButtonDrawFunc drawFunc;
	ButtonMouseDownFunc mouseDownFunc;
	ButtonMouseUpFunc mouseUpFunc;
	ButtonMouseMoveFunc mouseMoveFunc;

	Button();
	virtual ~Button();

public:
	bool GetDirty()
	{
		return dirty;
	}

	void SetDirty(bool value = true)
	{
		dirty = value;
	}

	const char* GetName()
	{
		return name;
	}
};
