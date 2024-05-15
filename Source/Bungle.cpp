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

	const auto header = tree->Data;

	if (header)
	{
		if (header->filename)
			delete[] header->filename;

		if (header->extraField)
			delete[] header->extraField;

		if (header->data)
			delete[] header->data;

		if (header->archiveName)
			delete[] header->archiveName;

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
		fread(&localFileHeader->signature, 4, 1, file);

		if ((localFileHeader->signature & 0xFFFF) != 0x4B50)
		{
			delete localFileHeader;
			return true;
		}

		fread(&localFileHeader->version, 2, 1, file);
		fread(&localFileHeader->flags, 2, 1, file);
		fread(&localFileHeader->compression, 2, 1, file);
		fread(&localFileHeader->modTime, 2, 1, file);
		fread(&localFileHeader->modDate, 2, 1, file);
		fread(&localFileHeader->crc32, 4, 1, file);
		fread(&localFileHeader->compressedSize, 4, 1, file);
		fread(&localFileHeader->uncompressedSize, 4, 1, file);
		fread(&localFileHeader->filenameLength, 2, 1, file);
		fread(&localFileHeader->extraFieldLength, 2, 1, file);

		localFileHeader->filename = nullptr;
		if (localFileHeader->filenameLength > 0)
		{
			localFileHeader->filename = new char[localFileHeader->filenameLength + 1];
			fread(localFileHeader->filename, localFileHeader->filenameLength, 1, file);
			localFileHeader->filename[localFileHeader->filenameLength] = 0;
		}

		localFileHeader->extraField = nullptr;
		if (localFileHeader->extraFieldLength > 0)
		{
			localFileHeader->extraField = new char[localFileHeader->extraFieldLength + 1];
			fread(localFileHeader->extraField, localFileHeader->extraFieldLength, 1, file);
			localFileHeader->extraField[localFileHeader->extraFieldLength] = 0;
		}

		localFileHeader->data = nullptr;
		if (localFileHeader->uncompressedSize > 0)
		{
			localFileHeader->data = new char[localFileHeader->uncompressedSize + 1];
			fread(localFileHeader->data, localFileHeader->uncompressedSize, 1, file);
			localFileHeader->data[localFileHeader->uncompressedSize] = 0;
		}

		localFileHeader->archiveName = nullptr;
		if (fileName)
		{
			localFileHeader->archiveName = new char[strlen(fileName) + 1];
			strcpy(localFileHeader->archiveName, fileName);
		}

		if ((localFileHeader->compression != 0) || localFileHeader->compressedSize != localFileHeader->uncompressedSize ||
			!localFileHeader->filename)
		{
			if (localFileHeader->filename)
				delete[] localFileHeader->filename;

			if (localFileHeader->extraField)
				delete[] localFileHeader->extraField;

			if (localFileHeader->data)
				delete[] localFileHeader->data;

			if (localFileHeader->archiveName)
				delete[] localFileHeader->archiveName;

			delete localFileHeader;
			continue;
		}

		sprintf(buffer, "%s%s", dirPath, localFileHeader->filename);
		BglSlashify(buffer);
		files.PutData(buffer, localFileHeader);
	}
}

bool BglFileLoaded(char* name)
{
	const auto slashifiedFilePath = new char[strlen(name) + 1];
	strcpy(slashifiedFilePath, name);
	BglSlashify(slashifiedFilePath);

	const auto fileHeader = files.GetData(slashifiedFilePath);
	delete[] slashifiedFilePath;
	
	return fileHeader != nullptr;
}

bool BglExtractFile(const char* filePath, const char* extractPath)
{
	const auto slashifiedFilePath = new char[strlen(filePath) + 1];
	strcpy(slashifiedFilePath, filePath);
	BglSlashify(slashifiedFilePath);

	const auto  fileHeader = files.GetData(slashifiedFilePath);
	delete[] slashifiedFilePath;

	if (fileHeader == nullptr)
		return false;

	const auto file = fopen(extractPath != nullptr ? extractPath : filePath, "wb");
	if (file == nullptr)
		return false;

	fwrite(fileHeader->data, fileHeader->uncompressedSize, 1, file);
	fclose(file);
	return true;
}

