#pragma once

#include <Button.hpp>

struct ScrollBox
{
	typedef void (*ScrollChangeCallback)(const char* name, int baseOffset);

	char Name[0x40];
	int X;
	int Y;
	int Width;
	int Height;
	int NumItems;
	int WindowSize;
	int CurrentIndex;
	ScrollChangeCallback Callback;
	bool InterfaceCreated;

	void SetName(const char* name);
	void SetDimensions(int x, int y, int width, int height);
	void SetNumItems(int num);
	void UpdateInterface();
	void SetWindowSize(int size);
	void SetCurrentIndex(int index);
	void SetCallback(ScrollChangeCallback callback);
	void CreateInterface();
	void RemoveInterface();
	void ScrollUp();
	void ScrollDown();
	static bool IsGrabInProgress();
	static void UpdateGrabScroll();
	static void GrabScrollBar(const char* name);
	static void UnGrabScrollBar();
	static ScrollBox* GetScrollBox(const char* name);
	static void CreateScrollBox(const char* name, int x, int y, int width, int height, int numItems, int windowSize, int currentIndex,
								ScrollChangeCallback callback);
	static void RemoveScrollBox(const char* name);
	static void ClickScrollUp(Button* button);
	static void ClickScrollDown(Button* button);
	static void ClickScrollBar(Button* button);
	static void MouseDownScrollBar(Button* button);
	static void MouseMoveScrollBar(Button* button);
	static void DrawScrollBar(Button* button, bool highlighted, bool clicked);
};
