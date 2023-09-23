#pragma once

#include "Source/Options.hpp"
#include "Source/Network.hpp"
#include "Source/MainMenu.hpp"
#include "Source/App.hpp"
#include "Source/Date.hpp"
#include "Source/Game.hpp"

static const auto VerifyLegitAndCodeCardCheck = (bool(*)())0x080FE6A0;
static const auto Init_OpenGL = (void(*)(int argc, char** argv))0x080FDC90;
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

static const auto Date_Update = (void(*)(Date*))0x08121e00;

static const auto Game_Game = (void(*)(Game*))0x0805bc20;
