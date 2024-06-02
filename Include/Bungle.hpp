#pragma once

#include <BTree.hpp>
#include <stdint.h>

struct LocalFileHeader
{
	uint32_t _signature;
	uint16_t _version;
	uint16_t _flags;
	uint16_t _compression;
	uint16_t _modTime;
	uint16_t _modDate;
	uint32_t _crc32;
	uint32_t _compressedSize;
	uint32_t _uncompressedSize;
	uint16_t _filenameLength;
	uint16_t _extraFieldLength;
	char* _filename;
	char* _extraField;
	char* _data;
	char* _archiveName;
};

void BglCloseAllFiles(BTree<LocalFileHeader*>* tree);
void BglCloseAllFiles();
bool BglOpenZipFile(FILE* file, const char* dirPath, const char* fileName);
bool BglFileLoaded(char* name);
bool BglExtractFile(const char* filePath, const char* extractPath);
DArray<char*>* BglListFiles(const char* path, const char* dir, const char* ext);
