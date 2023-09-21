#pragma once

#include "Source/Options.hpp"
#include "Source/Network.hpp"
#include "Source/MainMenu.hpp"
#include "Source/App.hpp"

static const auto RunUplinkExceptionHandling = (void(*)())0x080FEE90;
static const auto Init_Options = (void(*)(int argc, char** argv))0x080FE410;
static const auto VerifyLegitAndCodeCardCheck = (bool(*)())0x080FE6A0;
static const auto Load_Data = (bool(*)())0x080FE310;
static const auto Init_Game = (void(*)())0x080FE1D0;
static const auto Init_Graphics = (void(*)())0x080FDFC0;
static const auto Init_OpenGL = (void(*)(int argc, char** argv))0x080FDC90;
static const auto Init_Fonts = (void(*)())0x080FDD70;
static const auto Init_Sound = (void(*)())0x080FE170;
static const auto Init_Music = (void(*)())0x080FDFF0;
static const auto Run_MainMenu = (void(*)())0x080FDA70;
static const auto Run_Game = (void(*)())0x080FDA60;
static const auto Cleanup_Uplink = (void(*)())0x080FD9C0;
static const auto EclGetAccurateTime = (long double(*)())0x08177BB0;
static const auto FileReadDataInt = (bool(*)(const char*, int, void*, unsigned int, unsigned uint, FILE*))0x080568B0;
static const auto LoadBTree = (bool(*)(BTree<Option*>*, FILE*))0x08059460;
static const auto SaveBTree = (void(*)(BTree<Option*>*, FILE*))0x08058A70;

static const auto Options_CreateDefaultOptions = (void(*)(Options*))0x080FB980;

static const auto Network_Network = (void(*)(Network*))0x080F8C60;
static const auto MainMenu_MainMenu = (void(*)(MainMenu*))0x080EC870;

static const auto App_App = (void(*)(App*))0x0804ED90;
static const auto App_Update = (void(*)(App*))0x0804DAD0;
static const auto App_Initialise = (void(*)(App*))0x0804EBD0;
