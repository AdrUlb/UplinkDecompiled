#pragma once

#include <Button.hpp>

typedef void (*ClearDrawFunc)(int x, int y, int width, int height);
typedef void (*ButtonDrawFunc)(Button* button, bool highlighted, bool clicked);
typedef void (*ButtonMouseUpFunc)(Button* button);
typedef void (*ButtonMouseDownFunc)(Button* button);
typedef void (*ButtonMouseMoveFunc)(Button* button);
typedef void (*SuperhighlightDrawFunc)(Button*);

void EclEnableAnimations();
void EclDisableAnimations();
void EclEnableFasterAnimations(double speed);
void EclDisableFasterAnimations();
double EclGetAccurateTime();
void EclRegisterClearDrawFunction(ClearDrawFunc func);
void EclRegisterDefaultButtonCallbacks(ButtonDrawFunc draw, ButtonMouseUpFunc mouseUp, ButtonMouseDownFunc mouseDown, ButtonMouseMoveFunc mouseMove);
void EclRegisterSuperHighlightFunction(int borderWidth, SuperhighlightDrawFunc func);
void EclReset();
void EclRegisterButton(int x, int y, int width, int height, const char* caption, const char* name);
Button* EclGetButton(const char* name);
void EclDirtyButton();
void EclRegisterButtonCallbacks(const char* name, ButtonDrawFunc drawFunc, ButtonMouseUpFunc mouseUpFunc, ButtonMouseDownFunc mouseDownFunc,
								ButtonMouseMoveFunc mouseMoveFunc);