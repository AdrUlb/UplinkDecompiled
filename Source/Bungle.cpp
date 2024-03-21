#include <Bungle.hpp>

static BTree<LocalFileHeader*> files;

static void BglSlashify(char* path)
{
	for (auto slashChar = strchr(path, L'\\'); slashChar; slashChar = strchr(slashChar, L'\\'))
	{
		*slashChar = '/';
	}

	while (*path)
	{
		if (*path >= 'A' && *path <= 'Z')
			*path += ' ';

		path++;
	}
}

void BglCloseAllFiles(BTree<LocalFileHeader*>* tree)
{
	if (!tree)
		return;

	BglCloseAllFiles(tree->Left());
	BglCloseAllFiles(tree->Right());

	const auto header = tree->data;

	if (header)
	{
		if (header->Filename)
			delete[] header->Filename;

		if (header->ExtraField)
			delete[] header->ExtraField;

		if (header->Data)
			delete[] header->Data;

		if (header->ArchiveName)
			delete[] header->ArchiveName;

		delete header;
	}

	tree->Empty();
}

void BglCloseAllFiles()
{
	BglCloseAllFiles(&files);
	return;
}

bool BglOpenZipFile(FILE* file, const char* dirPath, const char* fileName)
{
	if (!file)
		return false;

	char buffer[0x100];

	while (true)
	{
		if (feof(file))
			return true;

		auto localFileHeader = new LocalFileHeader();
		memset(localFileHeader, 0, sizeof(LocalFileHeader));
		fread(localFileHeader, 4, 1, file);

		if ((localFileHeader->Signature & 0xFFFF) != 0x4B50)
		{
			delete localFileHeader;
			return true;
		}

		fread(&localFileHeader->Version, 2, 1, file);
		fread(&localFileHeader->Flags, 2, 1, file);
		fread(&localFileHeader->Compression, 2, 1, file);
		fread(&localFileHeader->ModTime, 2, 1, file);
		fread(&localFileHeader->ModDate, 2, 1, file);
		fread(&localFileHeader->Crc32, 4, 1, file);
		fread(&localFileHeader->CompressedSize, 4, 1, file);
		fread(&localFileHeader->UncompressedSize, 4, 1, file);
		fread(&localFileHeader->FilenameLength, 2, 1, file);
		fread(&localFileHeader->ExtraFieldLength, 2, 1, file);

		localFileHeader->Filename = nullptr;
		if (localFileHeader->FilenameLength > 0)
		{
			localFileHeader->Filename = new char[localFileHeader->FilenameLength + 1];
			fread(localFileHeader->Filename, localFileHeader->FilenameLength, 1, file);
			localFileHeader->Filename[localFileHeader->FilenameLength] = 0;
		}

		localFileHeader->ExtraField = nullptr;
		if (localFileHeader->ExtraFieldLength > 0)
		{
			localFileHeader->ExtraField = new char[localFileHeader->ExtraFieldLength + 1];
			fread(localFileHeader->ExtraField, localFileHeader->ExtraFieldLength, 1, file);
			localFileHeader->ExtraField[localFileHeader->ExtraFieldLength] = 0;
		}

		localFileHeader->Data = nullptr;
		if (localFileHeader->UncompressedSize > 0)
		{
			localFileHeader->Data = new char[localFileHeader->UncompressedSize + 1];
			fread(localFileHeader->Data, localFileHeader->UncompressedSize, 1, file);
			localFileHeader->Data[localFileHeader->UncompressedSize] = 0;
		}

		localFileHeader->ArchiveName = nullptr;
		if (fileName)
		{
			localFileHeader->ArchiveName = new char[strlen(fileName) + 1];
			strcpy(localFileHeader->ArchiveName, fileName);
		}

		if ((localFileHeader->Compression != 0) || localFileHeader->CompressedSize != localFileHeader->UncompressedSize ||
			!localFileHeader->Filename)
		{
			if (localFileHeader->Filename)
				delete[] localFileHeader->Filename;

			if (localFileHeader->ExtraField)
				delete[] localFileHeader->ExtraField;

			if (localFileHeader->Data)
				delete[] localFileHeader->Data;

			if (localFileHeader->ArchiveName)
				delete[] localFileHeader->ArchiveName;

			delete localFileHeader;
			continue;
		}

		sprintf(buffer, "%s%s", dirPath, localFileHeader->Filename);
		BglSlashify(buffer);
		files.PutData(buffer, localFileHeader);
		continue;
	}
}