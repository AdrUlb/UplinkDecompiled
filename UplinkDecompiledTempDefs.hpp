#pragma once

#include "Source/BTree.hpp"

typedef void Options;
typedef void Network;
typedef void MainMenu;
typedef void PhoneDialler;

static const auto gFilesPtr = (BTree<LocalFileHeader*>*)0x082070B0;
#define gFiles (*gFilesPtr)

static const auto gRsInitialisedPtr = (bool*)0x082070A9;
static const auto gRsAppPath = (char*)0x08205DE0;
static const auto gRsTempDir = (char*)0x08205EE0;
#define gRsInitialised (*gRsInitialisedPtr)

static const auto RunUplinkExceptionHandling = (void(*)())0x080FEE90;
static const auto Init_App = (void(*)(const char *exeFilePath))0x080FEAA0;
static const auto Init_Options = (void(*)(int argc,char **argv))0x080FE410;
static const auto VerifyLegitAndCodeCardCheck = (bool(*)())0x080FE6A0;
static const auto Load_Data = (bool(*)())0x080FE310;
static const auto Init_Game = (void(*)())0x080FE1D0;
static const auto Init_Graphics = (void(*)())0x080FDFC0;
static const auto Init_OpenGL = (void(*)(int argc,char **argv))0x080FDC90;
static const auto Init_Fonts = (void(*)())0x080FDD70;
static const auto Init_Sound = (void(*)())0x080FE170;
static const auto Init_Music = (void(*)())0x080FDFF0;
static const auto Run_MainMenu = (void(*)())0x080FDA70;
static const auto Run_Game = (void(*)())0x080FDA60;
static const auto Cleanup_Uplink = (void(*)())0x080FD9C0;
