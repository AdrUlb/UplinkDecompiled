#pragma once

#include <cstdio>
#include <cstdint>

struct LocalFileHeader
{
	char Signature[4];
	uint16_t Version = 0;
	uint16_t Flags = 0;
	uint16_t CompressionMethod = 0;
	uint16_t LastModifiedTime = 0;
	uint16_t LastModifiedDate = 0;
	uint32_t Crc32 = 0;
	uint32_t CompressedSize = 0;
	uint32_t UncompressedSize = 0;
	uint16_t FileNameLength = 0;
	uint16_t ExtraFieldLength = 0;
	char* FileName = nullptr;
	char* ExtraField = nullptr;
	char* Data = nullptr;
	char* ZipFileName = nullptr;
};

bool BglOpenZipFile(FILE* file, const char* appPath, const char* fileName);
void BglCloseAllFiles();