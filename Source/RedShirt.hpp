#pragma once

#include <cstdio>

bool RsFileEncrypted(const char* filePath);
bool RsFileEncryptedNoVerify(const char* filePath);
FILE* RsFileOpen(const char* filePath, const char* mode);
void RsFileClose(const char* fileName, FILE* file);
bool RsLoadArchive(const char* fileName);
void RsInitialise(const char* appPath);
