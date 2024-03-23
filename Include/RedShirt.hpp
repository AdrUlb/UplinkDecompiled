#pragma once

#include <cstdio>

void RsInitialise(const char* appPath);
void RsCleanUp();
bool RsFileEncryptedNoVerify(const char* path);
bool RsFileEncrypted(const char* path);
FILE* RsFileOpen(const char* path, const char* modes);
void RsFileClose(const char* filePath, FILE* file);
bool RsLoadArchive(const char* name);
