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
		if (header->_filename)
			delete[] header->_filename;

		if (header->_extraField)
			delete[] header->_extraField;

		if (header->_data)
			delete[] header->_data;

		if (header->_archiveName)
			delete[] header->_archiveName;

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
		fread(&localFileHeader->_signature, 4, 1, file);

		if ((localFileHeader->_signature & 0xFFFF) != 0x4B50)
		{
			delete localFileHeader;
			return true;
		}

		fread(&localFileHeader->_version, 2, 1, file);
		fread(&localFileHeader->_flags, 2, 1, file);
		fread(&localFileHeader->_compression, 2, 1, file);
		fread(&localFileHeader->_modTime, 2, 1, file);
		fread(&localFileHeader->_modDate, 2, 1, file);
		fread(&localFileHeader->_crc32, 4, 1, file);
		fread(&localFileHeader->_compressedSize, 4, 1, file);
		fread(&localFileHeader->_uncompressedSize, 4, 1, file);
		fread(&localFileHeader->_filenameLength, 2, 1, file);
		fread(&localFileHeader->_extraFieldLength, 2, 1, file);

		localFileHeader->_filename = nullptr;
		if (localFileHeader->_filenameLength > 0)
		{
			localFileHeader->_filename = new char[localFileHeader->_filenameLength + 1];
			fread(localFileHeader->_filename, localFileHeader->_filenameLength, 1, file);
			localFileHeader->_filename[localFileHeader->_filenameLength] = 0;
		}

		localFileHeader->_extraField = nullptr;
		if (localFileHeader->_extraFieldLength > 0)
		{
			localFileHeader->_extraField = new char[localFileHeader->_extraFieldLength + 1];
			fread(localFileHeader->_extraField, localFileHeader->_extraFieldLength, 1, file);
			localFileHeader->_extraField[localFileHeader->_extraFieldLength] = 0;
		}

		localFileHeader->_data = nullptr;
		if (localFileHeader->_uncompressedSize > 0)
		{
			localFileHeader->_data = new char[localFileHeader->_uncompressedSize + 1];
			fread(localFileHeader->_data, localFileHeader->_uncompressedSize, 1, file);
			localFileHeader->_data[localFileHeader->_uncompressedSize] = 0;
		}

		localFileHeader->_archiveName = nullptr;
		if (fileName)
		{
			localFileHeader->_archiveName = new char[strlen(fileName) + 1];
			strcpy(localFileHeader->_archiveName, fileName);
		}

		if ((localFileHeader->_compression != 0) || localFileHeader->_compressedSize != localFileHeader->_uncompressedSize || !localFileHeader->_filename)
		{
			if (localFileHeader->_filename)
				delete[] localFileHeader->_filename;

			if (localFileHeader->_extraField)
				delete[] localFileHeader->_extraField;

			if (localFileHeader->_data)
				delete[] localFileHeader->_data;

			if (localFileHeader->_archiveName)
				delete[] localFileHeader->_archiveName;

			delete localFileHeader;
			continue;
		}

		sprintf(buffer, "%s%s", dirPath, localFileHeader->_filename);
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

	const auto fileHeader = files.GetData(slashifiedFilePath);
	delete[] slashifiedFilePath;

	if (fileHeader == nullptr)
		return false;

	const auto file = fopen(extractPath != nullptr ? extractPath : filePath, "wb");
	if (file == nullptr)
		return false;

	fwrite(fileHeader->_data, fileHeader->_uncompressedSize, 1, file);
	fclose(file);
	return true;
}

DArray<char*>* BglListFiles(const char* path, const char* dir, const char* ext)
{
	char searchDir[0x100];
	sprintf(searchDir, "%s%s", path, dir);
	BglSlashify(searchDir);

	const auto fileList = files.ConvertIndexToDArray();

	for (int i = 0; i < fileList->Size(); i++)
	{
		if (fileList->ValidIndex(i))
		{
			const auto filePath = fileList->GetData(i);
			char currentDir[0x100];
			strncpy(currentDir, filePath, strlen(searchDir));
			currentDir[strlen(searchDir)] = 0;

			bool keep = true;

			if (strcmp(currentDir, searchDir) != 0)
				keep = false;

			if (strstr(filePath, ext) == nullptr)
				keep = false;

			if (!keep)
				fileList->RemoveData(i);
		}
	}

	return fileList;
}
