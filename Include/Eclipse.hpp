#pragma once

#include <Animation.hpp>
#include <Button.hpp>

typedef void (*ClearDrawFunc)(int x, int y, int width, int height);
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
bool EclIsButtonEditable(const char* name);
void EclHighlightButton(const char* name);
void EclUnHighlightButton();
void EclMakeButtonEditable(const char* name);
void EclMakeButtonUnEditable(const char* name);
void EclDrawAllButtons();
void EclRegisterButton(int x, int y, int width, int height, const char* caption, const char* name);
void EclRegisterButton(int x, int y, int width, int height, const char* caption, const char* tooltip, const char* name);
void EclRemoveButton(const char* name);
void EclRemoveAnimation(int index);
Button* EclGetButton(const char* name);
void EclDirtyRectangle();
void EclRegisterButtonCallbacks(const char* name, ButtonDrawFunc drawFunc, ButtonMouseUpFunc mouseUpFunc, ButtonMouseDownFunc mouseDownFunc,
								ButtonMouseMoveFunc mouseMoveFunc);
void EclClearRectangle(int x, int y, int width, int height);
void EclUpdateAllAnimations();
int EclRegisterCaptionChange(const char* buttonName, const char* caption, int time, AnimationFinishedCallback finishedCallback);
int EclRegisterCaptionChange(const char* buttonName, const char* caption, AnimationFinishedCallback finishedCallback);
int EclRegisterMovement(const char* buttonName, int x, int y, int time, AnimationFinishedCallback callback);
void EclButtonBringToFront(const char* name);
void EclButtonSendToBack(const char* name);
void EclRegisterButtonCallback(const char* name, ButtonMouseUpFunc func);
const char* EclGetButtonAtCoord(int x, int y);
void EclClickButton(const char* name);
void EclUnClickButton();
char const* EclGetHighlightedButton();
void GciSaveScreenshot(const char* filePath);
void EclHighlightNextEditableButton();
void textbutton_keypress(Button* button, char c);