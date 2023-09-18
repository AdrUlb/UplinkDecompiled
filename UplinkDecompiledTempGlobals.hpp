#pragma once

#include "Source/BTree.hpp"
#include "Source/App.hpp"

static const auto gRsAppPath = (char*)0x08205DE0;
static const auto gRsTempDir = (char*)0x08205EE0;

static const auto gFilesPtr = (BTree<LocalFileHeader*>*)0x082070B0;
static const auto gRsInitialisedPtr = (bool*)0x082070A9;
static const auto gAppPtr = (App**)0x08206304;
static const auto gFileStdoutPtr = (FILE**)0x08206A24;

#define gFiles (*gFilesPtr)
#define gRsInitialised (*gRsInitialisedPtr)
#define gApp (*gAppPtr)
#define gFileStdout (*gFileStdoutPtr)
