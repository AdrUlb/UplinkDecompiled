#pragma once

#include <SDL/SDL.h>
#include "Uplink/BTree.hpp"
#include "Uplink/App.hpp"
#include "Uplink/Game.hpp"
#include "Uplink/Button.hpp"

static const auto gRsAppPath = (char*)0x08205DE0;
static const auto gRsTempDir = (char*)0x08205EE0;
static const auto gFilesPtr = (BTree<LocalFileHeader*>*)0x082070B0;
static const auto gRsInitialisedPtr = (bool*)0x082070A9;
static const auto gAppPtr = (App**)0x08206304;
static const auto gGamePtr = (Game**)0x0820657C;
static const auto gFileStdoutPtr = (FILE**)0x08206A24;
static const auto gWindowScaleXPtr = (float*)0x08204948;
static const auto gWindowScaleYPtr = (float*)0x08204944;
static const auto gSgInitialisedPtr = (bool*)0x08206f74;
static const auto gGciIsInitGraphicsLibraryPtr = (bool*)0x08206FAC;
static const auto gScreenPtr = (SDL_Surface**)0x08206FA8;
static const auto gCurrentHighlightPtr = (char**)0x08207040;
static const auto gCurrentClickPtr = (char**)0x08207040;
static const auto gEditableButtonsPtr = (LList<char*>*)0x0820702C;
static const auto gButtonsPtr = (LList<Button*>*)0x0820702C;
static const auto gSuperhighlightBorderWidthPtr = (int*)0x08207058;

#define gFiles (*gFilesPtr)
#define gRsInitialised (*gRsInitialisedPtr)
#define gApp (*gAppPtr)
#define gGame (*gGamePtr)
#define gFileStdout (*gFileStdoutPtr)
#define gWindowScaleX (*gWindowScaleXPtr)
#define gWindowScaleY (*gWindowScaleYPtr)
#define gSgInitialised (*gSgInitialisedPtr)
#define gGciIsInitGraphicsLibrary (*gGciIsInitGraphicsLibraryPtr)
#define gScreen (*gScreenPtr)
#define gCurrentHighlight (*gCurrentHighlightPtr)
#define gCurrentClick (*gCurrentClickPtr)
#define gEditableButtons (*gEditableButtonsPtr)
#define gButtons (*gButtonsPtr)
#define gSuperhighlightBorderWidth (*gSuperhighlightBorderWidthPtr)
