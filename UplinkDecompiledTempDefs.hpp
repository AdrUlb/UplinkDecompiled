#pragma once

#include "Source/Options.hpp"
#include "Source/Network.hpp"
#include "Source/MainMenu.hpp"
#include "Source/App.hpp"
#include "Source/Date.hpp"
#include "Source/Game.hpp"

typedef void Button;
typedef void(*ClearDrawFunc)(int, int, int, int);
typedef void(*ButtonDrawFunc)(Button*, bool, bool);
typedef void(*ButtonMouseUpFunc)(Button*);
typedef void(*ButtonMouseDownFunc)(Button*);
typedef void(*ButtonMouseMoveFunc)(Button*);

static const auto VerifyLegitAndCodeCardCheck = (bool(*)())0x080FE6A0;
//static const auto Init_OpenGL = (void(*)(int argc, char** argv))0x080FDC90;
static const auto Init_Fonts = (void(*)())0x080FDD70;
static const auto Init_Music = (void(*)())0x080FDFF0;
static const auto Run_MainMenu = (void(*)())0x080FDA70;
static const auto Run_Game = (void(*)())0x080FDA60;
static const auto EclGetAccurateTime = (long double(*)())0x08177BB0;
static const auto FileReadDataInt = (bool(*)(const char*, int, void*, unsigned int, unsigned uint, FILE*))0x080568B0;
static const auto LoadBTree = (bool(*)(BTree<Option*>*, FILE*))0x08059460;
static const auto SaveBTree = (void(*)(BTree<Option*>*, FILE*))0x08058A70;

static const auto GciRestoreScreenSize = (void(*)())0x08176100;

static const auto Options_CreateDefaultOptions = (void(*)(Options*))0x080FB980;
static const auto Options_Print = (void(*)(Options*))0x080fabe0;
static const auto Options_SetThemeName = (void(*)(Options*, const char*))0x080fc480;
static const auto Options_Load = (bool(*)(Options*, FILE*))0x080fb070;

static const auto Network_Network = (void(*)(Network*))0x080F8C60;

static const auto MainMenu_MainMenu = (void(*)(MainMenu*))0x080EC870;

static const auto App_App = (void(*)(App*))0x0804ED90;
static const auto App_Update = (void(*)(App*))0x0804DAD0;
static const auto App_Initialise = (void(*)(App*))0x0804EBD0;
static const auto App_Close = (void(*)(App*))0x0804d5b0;
static const auto App_Print = (void(*)(App*))0x0804d060;
static const auto App_CoreDump = (void(*)())0x0804D130;

static const auto Date_Update = (void(*)(Date*))0x08121e00;

static const auto Game_Game = (void(*)(Game*))0x0805bc20;

static const auto GciInitGraphics = (char* (*)(const char*, int, int, int, int, int, int, char**))0x08176680;
static const auto GciGetCurrentScreenMode = (void* (*)(void))0x08175FE0;
static const auto EclReset = (void(*)(int, int))0x08179250;
static const auto EclRegisterClearDrawFunction = (void(*)(ClearDrawFunc func))0x08177770;
static const auto clear_draw = (void(*)(int, int, int, int))0x08052AE0;
static const auto EclRegisterDefaultButtonCallbacks = (void(*)(ButtonDrawFunc, ButtonMouseUpFunc, ButtonMouseDownFunc, ButtonMouseMoveFunc))0x081776F0;
static const auto button_draw = (void(*)(Button*, bool, bool))0x08054490;
static const auto button_click = (void(*)(Button*))0x08053840;
static const auto button_highlight = (void(*)(Button*))0x080538B0;
static const auto EclRegisterSuperHighlightFunction = (void(*)(int, ButtonDrawFunc))0x08177780;
static const auto superhighlight_draw = (void(*)(Button*, bool, bool))0x080539A0;
static const auto EclDisableAnimations = (void(*)())0x081777C0;
static const auto EclEnableFasterAnimations = (void(*)(double))0x081777D0;
static const auto setcallbacks = (void(*)())0x08050BF0;
