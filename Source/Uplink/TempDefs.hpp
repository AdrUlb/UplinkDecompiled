#pragma once

#include "Include/Options.hpp"
#include "Include/Network.hpp"
#include "Include/MainMenu.hpp"
#include "Include/App.hpp"
#include "Include/Date.hpp"
#include "Include/Game.hpp"
#include "Include/Button.hpp"

typedef void(*ClearDrawFunc)(int, int, int, int);

#if UPLINKDECOMP32
static const auto VerifyLegitAndCodeCardCheck = (bool(*)())0x080FE6A0;
static const auto Init_Fonts = (void(*)())0x080FDD70;
static const auto Init_Music = (void(*)())0x080FDFF0;
static const auto Run_MainMenu = (void(*)())0x080FDA70;
static const auto Run_Game = (void(*)())0x080FDA60;
static const auto CreateUplinkObject = (UplinkObject * (*)(int objectId))0x080572B0;

static const auto Options_CreateDefaultOptions = (void(*)(Options*))0x080FB980;
static const auto Options_Print = (void(*)(Options*))0x080fabe0;
static const auto Options_SetThemeName = (void(*)(Options*, const char*))0x080fc480;
static const auto Options_Load = (bool(*)(Options*, FILE*))0x080fb070;

static const auto Network_Network = (void(*)(Network*))0x080F8C60;

static const auto MainMenu_MainMenu = (void(*)(MainMenu*))0x080EC870;
static const auto MainMenu__MainMenu = (void(*)(MainMenu*))0x080EC7D0;

static const auto App_Update = (void(*)(App*))0x0804DAD0;
static const auto App_Initialise = (void(*)(App*))0x0804EBD0;
static const auto App_Close = (void(*)(App*))0x0804d5b0;
static const auto App_Print = (void(*)(App*))0x0804d060;
static const auto App_CoreDump = (void(*)())0x0804D130;

static const auto Date_Update = (void(*)(Date*))0x08121e00;

static const auto Game_Game = (void(*)(Game*))0x0805bc20;
static const auto Game_Load = (bool(*)(Game*, FILE*))0x0805AF20;
static const auto Game_Save = (void(*)(Game*, FILE*))0x0805ACD0;
static const auto Game_Print = (void(*)(Game*))0x0805AC20;
static const auto Game_Update = (void(*)(Game*))0x0805ab80;

static const auto World_World = (void(*)(World*))0x08171C30;
static const auto World__World = (void(*)(World*))0x08171170;

static const auto clear_draw = (void(*)(int, int, int, int))0x08052AE0;
static const auto button_draw = (void(*)(Button*, bool, bool))0x08054490;
static const auto button_click = (void(*)(Button*))0x08053840;
static const auto button_highlight = (void(*)(Button*))0x080538B0;
static const auto superhighlight_draw = (void(*)(Button*, bool, bool))0x080539A0;
static const auto setcallbacks = (void(*)())0x08050BF0;
static const auto EclRemoveButton = (void(*)(char*))0x081790E0;

#elif UPLINKDECOMP64
static const auto VerifyLegitAndCodeCardCheck = (bool(*)())0x004A07E0;
static const auto Init_Fonts = (void(*)())0x0049FFA0;
static const auto Init_Music = (void(*)())0x004A0210;
static const auto Run_MainMenu = (void(*)())0x0049FD40;
static const auto Run_Game = (void(*)())0x0049FD40;
static const auto CreateUplinkObject = (UplinkObject * (*)(int objectId))0x0040FF10;

static const auto Options_CreateDefaultOptions = (void(*)(Options*))0x0049DE30;
static const auto Options_Print = (void(*)(Options*))0x0049D140;
static const auto Options_SetThemeName = (void(*)(Options*, const char*))0x0049E740;
static const auto Options_Load = (bool(*)(Options*, FILE*))0x0049D530;

static const auto Network_Network = (void(*)(Network*))0x0049b460;

static const auto MainMenu_MainMenu = (void(*)(MainMenu*))0x004910C0;
static const auto MainMenu__MainMenu = (void(*)(MainMenu*))0x00491020;

static const auto App_Update = (void(*)(App*))0x004076F0;
static const auto App_Initialise = (void(*)(App*))0x004085e0;
static const auto App_Close = (void(*)(App*))0x00407230;
static const auto App_Print = (void(*)(App*))0x00406D30;
static const auto App_CoreDump = (void(*)())0x00406DF0;

static const auto Date_Update = (void(*)(Date*))0x004C0E90;

static const auto Game_Game = (void(*)(Game*))0x004141D0;
static const auto Game_Load = (bool(*)(Game*, FILE*))0x00413660;
static const auto Game_Save = (void(*)(Game*, FILE*))0x00413470;
static const auto Game_Print = (void(*)(Game*))0x004133D0;
static const auto Game_Update = (void(*)(Game*))0x00413340;

static const auto clear_draw = (void(*)(int, int, int, int))0x0040BEF0;
static const auto button_draw = (void(*)(Button*, bool, bool))0x0040D6B0;
static const auto button_click = (void(*)(Button*))0x0040CBC0;
static const auto button_highlight = (void(*)(Button*))0x0040CC20;
static const auto superhighlight_draw = (void(*)(Button*, bool, bool))0x0040CCF0;
static const auto setcallbacks = (void(*)())0x0040A4B0;
static const auto EclRemoveButton = (void(*)(char*))0x0050EEE0;
#else
#error UNKNOWN BUILD ARCHITECTURE
#endif
