#include "Bungle.hpp"

#include <cstring>
#include <climits>
#include "BTree.hpp"
#include "../UplinkDecompiledTempGlobals.hpp"

static void BglSlashify(char* filePath)
{
	for (auto pcVar1 = strchr(filePath, L'\\'); pcVar1; pcVar1 = strchr(pcVar1, L'\\'))
		*pcVar1 = '/';

	for (auto c = *filePath; c != 0; c = *(++filePath))
	{
		if (c >= 'A' && c <= 'Z')
			c += 'a' - 'A';

		*filePath = c;
	}
}

static void BglCloseAllFiles(BTree<LocalFileHeader*>* filesToClose)
{
	if (filesToClose)
	{
		BglCloseAllFiles(filesToClose->Left());
		BglCloseAllFiles(filesToClose->Right());
		const auto localFileHeader = filesToClose->Data;
		if (localFileHeader)
		{
			if (localFileHeader->FileName)
				delete[] localFileHeader->FileName;

			if (localFileHeader->ExtraField)
				delete[] localFileHeader->ExtraField;

			if (localFileHeader->Data)
				delete[] localFileHeader->Data;

			if (localFileHeader->ZipFileName)
				delete[] localFileHeader->ZipFileName;

			delete(localFileHeader);
		}
		filesToClose->Empty();
		return;
	}
	return;
}

bool BglOpenZipFile(FILE* file, const char* appPath, const char* fileName)
{
	if (!file)
		return false;

	while (!feof(file))
	{
		auto localFileHeader = new LocalFileHeader();

		fread(&localFileHeader->Signature, 4, 1, file);

		if (localFileHeader->Signature[0] != 'P' || localFileHeader->Signature[1] != 'K')
		{
			delete localFileHeader;
			return true;
		}

		fread(&localFileHeader->Version, 2, 1, file);
		fread(&localFileHeader->Flags, 2, 1, file);
		fread(&localFileHeader->CompressionMethod, 2, 1, file);
		fread(&localFileHeader->LastModifiedTime, 2, 1, file);
		fread(&localFileHeader->LastModifiedDate, 2, 1, file);
		fread(&localFileHeader->Crc32, 4, 1, file);
		fread(&localFileHeader->CompressedSize, 4, 1, file);
		fread(&localFileHeader->UncompressedSize, 4, 1, file);
		fread(&localFileHeader->FileNameLength, 2, 1, file);
		fread(&localFileHeader->ExtraFieldLength, 2, 1, file);

		if (localFileHeader->FileNameLength > 0)
		{
			localFileHeader->FileName = new char[localFileHeader->FileNameLength + 1];
			fread(localFileHeader->FileName, localFileHeader->FileNameLength, 1, file);
			localFileHeader->FileName[localFileHeader->FileNameLength] = 0;
		}
		else
			localFileHeader->FileName = nullptr;

		if (localFileHeader->ExtraFieldLength > 0)
		{
			localFileHeader->ExtraField = new char[localFileHeader->ExtraFieldLength + 1];
			fread(localFileHeader->ExtraField, localFileHeader->ExtraFieldLength, 1, file);
			localFileHeader->ExtraField[localFileHeader->ExtraFieldLength] = 0;
		}
		else
			localFileHeader->ExtraField = nullptr;

		if (localFileHeader->UncompressedSize > 0)
		{
			localFileHeader->Data = new char[localFileHeader->UncompressedSize + 1];
			fread(localFileHeader->Data, localFileHeader->UncompressedSize, 1, file);
			localFileHeader->Data[localFileHeader->UncompressedSize] = 0;
		}
		else
			localFileHeader->Data = nullptr;

		if (fileName)
		{
			localFileHeader->ZipFileName = new char[strlen(fileName) + 1];
			strcpy(localFileHeader->ZipFileName, fileName);
		}
		else
			localFileHeader->ZipFileName = nullptr;

		if (!localFileHeader->FileName || localFileHeader->CompressionMethod != 0 || localFileHeader->CompressedSize != localFileHeader->UncompressedSize)
		{
			if (localFileHeader->FileName)
				delete[] localFileHeader->FileName;

			if (localFileHeader->ExtraField)
				delete[] localFileHeader->ExtraField;

			if (localFileHeader->Data)
				delete[] localFileHeader->Data;

			if (localFileHeader->ZipFileName)
				delete[] localFileHeader->ZipFileName;

			delete localFileHeader;
			continue;
		}

		char filePath[PATH_MAX];
		sprintf(filePath, "%s%s", appPath, localFileHeader->FileName);
		BglSlashify(filePath);
		gFiles.PutData(filePath, &localFileHeader);
	}

	return true;
}

void BglCloseAllFiles()
{
	BglCloseAllFiles(&gFiles);
}
