#pragma once

#include <BTree.hpp>
#include <stdint.h>

struct LocalFileHeader
{
	uint32_t signature;
	uint16_t version;
	uint16_t flags;
	uint16_t compression;
	uint16_t modTime;
	uint16_t modDate;
	uint32_t crc32;
	uint32_t compressedSize;
	uint32_t uncompressedSize;
	uint16_t filenameLength;
	uint16_t extraFieldLength;
	char* filename;
	char* extraField;
	char* data;
	char* archiveName;
};

void BglCloseAllFiles(BTree<LocalFileHeader*>* tree);
void BglCloseAllFiles();
bool BglOpenZipFile(FILE* file, const char* dirPath, const char* fileName);
