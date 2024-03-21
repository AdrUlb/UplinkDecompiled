#pragma once

#include <stdint.h>
#include <BTree.hpp>

struct LocalFileHeader
{
	uint32_t Signature;
	uint16_t Version;
	uint16_t Flags;
	uint16_t Compression;
	uint16_t ModTime;
	uint16_t ModDate;
	uint32_t Crc32;
	uint32_t CompressedSize;
	uint32_t UncompressedSize;
	uint16_t FilenameLength;
	uint16_t ExtraFieldLength;
	char* Filename;
	char* ExtraField;
	char* Data;
	char* ArchiveName;
};

void BglCloseAllFiles();
