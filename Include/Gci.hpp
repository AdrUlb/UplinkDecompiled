#pragma once

#include <cstdint>

enum class GciMouseEvent
{
	Unknown = -1,
	Up,
	Down
};

enum class GciMouseButton
{
	Unknown = -1,
	Left = 1,
	Right,
	Middle
};

struct GciInitFlags
{
	bool UnknownFlag0 : 1 {false};
	bool UnknownFlag1 : 1 {false};
	bool Fullscreen : 1 {false};
	bool Debug : 1 {false};
};

typedef void (*DisplayFunc)();
typedef void (*MouseFunc)(GciMouseButton button, GciMouseEvent event, int x, int y);
typedef void (*MotionFunc)(int x, int y);
typedef void (*PassiveMotionFunc)(int x, int y);
typedef void (*KeyboardFunc)(char keychar);
typedef void (*KeyboardSpecialFunc)(int keycode);
typedef void (*IdleFunc)();
typedef void (*ReshapeFunc)(int width, int height);

typedef void (*GciTimerCallback)(int);

void GciDisplayFunc(DisplayFunc func);
void GciMouseFunc(MouseFunc func);
void GciMotionFunc(MotionFunc func);
void GciPassiveMotionFunc(PassiveMotionFunc func);
void GciKeyboardFunc(KeyboardFunc func);
void GciSpecialFunc(KeyboardSpecialFunc func);
void GciIdleFunc(IdleFunc func);
void GciReshapeFunc(ReshapeFunc func);
void GciRestoreScreenSize();
const char* GciInitGraphicsLibrary(GciInitFlags flags);
const char* GciInitGraphics(const char* title, GciInitFlags flags, int width, int height, int depth);
int* GciGetClosestScreenMode(int width, int height);
int* GciGetCurrentScreenMode();
void GciEnableTrueTypeSupport();
void GciDisableTrueTypeSupport();
bool GciRegisterTrueTypeFont();
bool GciUnregisterTrueTypeFont();
void GciSetDefaultFont(int index);
void GciDeleteTrueTypeFont(int index);
bool GciLoadTrueTypeFont(int index, const char* name, const char* path, int size);
void GciTimerFunc(int delay, GciTimerCallback callback, int arg);
void GciMainLoop();
bool GciAppVisible();
void GciSwapBuffers();
int GciTextWidth(char const* text, int font);
int GciTextWidth(const char* text);
void GciDrawText(int x, int y, const char* text, int font);
void GciDrawText(int x, int y, const char* text);