#pragma once

#include <cstdio>

void RsInitialise(const char* appPath);
void RsCleanUp();
unsigned int RsFileCheckSum(FILE* file, void* buffer, size_t size);
bool RsFileEncryptedNoVerify(const char* path);
bool RsFileEncrypted(const char* path);
FILE* RsFileOpen(const char* path, const char* modes);
void RsFileClose(const char* filePath, FILE* file);
bool RsLoadArchive(const char* name);
bool RsFileExists(const char* path);
const char* RsArchiveFileOpen(const char* filePath);
int RsArchiveFileClose(const char* filePath, FILE* file);
bool RsEncryptFile(const char* path);
