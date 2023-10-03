#pragma once

#include <Button.hpp>

typedef void(*ClearDrawFunc)(int, int, int, int);

void EclReset(int width, int height);
void EclRegisterClearDrawFunction(ClearDrawFunc func);
void EclRegisterDefaultButtonCallbacks(ButtonDrawFunc drawFunc, ButtonMouseUpFunc mouseUpFunc,
	ButtonMouseDownFunc mouseDownFunc, ButtonMouseMoveFunc mouseMoveFunc);
void EclRegisterSuperHighlightFunction(int borderWidth, ButtonDrawFunc func);
void EclDisableAnimations();
void EclEnableFasterAnimations(double speed);
long double EclGetAccurateTime();
