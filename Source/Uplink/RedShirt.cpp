#include "RedShirt.hpp"

#include <cstring>
#include <cstdlib>
#include "Bungle.hpp"
#include "../UplinkDecompiledTempGlobals.hpp"
#include "../UplinkDecompiledTempDefs.hpp"
#include "../../../unrar/rar.hpp"
#include "../../../unrar/sha1.hpp"

typedef void(*FilterCallback)(unsigned char* buffer, size_t size);
typedef bool(*FilterFileCallback)(FILE* file);

static inline size_t HashResultSize()
{
	return 0x14;
}

static sha1_context* HashInitial()
{
	auto context = new sha1_context();
	sha1_init(context);
	return context;
}

static void HashData(sha1_context* context, const byte* data, size_t length)
{
	sha1_process(context, data, length);
}

static size_t HashFinal(sha1_context* context, byte* data, size_t length)
{
	uint digest[5];

	sha1_done(context, digest);

	delete context;

	if (length > 0x13)
		length = 0x14;

	memcpy(data, digest, length);

	return length;
}

static size_t RsFileCheckSum(FILE* file, byte* data, uint length)
{
	byte buffer[16384];

	auto context = HashInitial();
	while (true)
	{
		const auto readCount = fread(buffer, 1, 0x4000, file);
		if (readCount == 0)
			break;
		HashData(context, buffer, readCount);
	}

	return HashFinal(context, data, length);
}

static bool filterStream(FILE* sourceFile, FILE* destFile, FilterCallback filterCallback)
{
	size_t bytesRead;
	size_t bytesWritten;
	unsigned char buffer[0x4000];

	do
	{
		bytesRead = fread(buffer, 1, 0x4000, sourceFile);

		if (bytesRead == 0)
			return true;

		filterCallback(buffer, bytesRead);

		bytesWritten = fwrite(buffer, 1, bytesRead, destFile);
	} while (bytesWritten >= bytesRead);

	return false;
}

static bool filterFile(const char* sourceFilePath, const char* destFilePath, FilterFileCallback beforeReadCallback,
	FilterFileCallback beforeWriteCallback, FilterFileCallback afterWriteCallback, FilterCallback filterCallback)
{
	const auto file = fopen(sourceFilePath, "rb");

	if (!file)
		return false;

	if (!beforeReadCallback(file))
	{
		puts("redshirt: failed to read header!");
		fclose(file);
		return false;
	}

	auto tempFile = fopen(destFilePath, "w+b");
	if (!tempFile)
	{
		fclose(file);
		return false;
	}

	if (!beforeWriteCallback(tempFile))
	{
		puts("redshirt: failed to write header!");
		fclose(file);
		fclose(tempFile);
		remove(destFilePath);
		return false;
	}

	if (!filterStream(file, tempFile, filterCallback))
	{
		puts("redshirt: failed to write containning bytes!");
		fclose(file);
		fclose(tempFile);
		remove(destFilePath);
		return false;
	}

	if (!afterWriteCallback(tempFile))
	{
		puts("redshirt: failed to write checksum!");
		fclose(file);
		fclose(tempFile);
		remove(destFilePath);
		return false;
	}

	fclose(file);
	fclose(tempFile);
	return true;
}

static bool filterFileInPlace(const char* sourceFilePath, const char* destFileSuffix, FilterFileCallback beforeReadCallback,
	FilterFileCallback beforeWriteCallback, FilterFileCallback afterWriteCallback, FilterCallback filterCallback)
{
	char destFilePath[256];

	sprintf(destFilePath, "%s%s", sourceFilePath, destFileSuffix);
	if (!filterFile(sourceFilePath, destFilePath, beforeReadCallback, beforeWriteCallback, afterWriteCallback, filterCallback))
	{
		puts("Redshirt ERROR : Failed to write output file");
		return false;
	}

	remove(sourceFilePath);
	rename(destFilePath, sourceFilePath);
	return true;
}

static void encryptBuffer(unsigned char* buffer, size_t size)
{
	for (size_t i = 0; i < size; i++)
		buffer[i] += 0x80;
}

static void decryptBuffer(unsigned char* buffer, size_t size)
{
	for (size_t i = 0; i < size; i++)
		buffer[i] += 0x80;
}

static bool readRsEncryptedHeader(FILE* file)
{
	char buffer[9];

	if (fread(buffer, 9, 1, file) != 1)
		return false;

	if (strcmp(buffer, "REDSHRT2") == 0)
	{
		auto hashResultSize = HashResultSize();
		char hashResult[hashResultSize];
		auto read = fread(hashResult, hashResultSize, 1, file);

		return read == 1;
	}

	return strcmp(buffer, "REDSHIRT") == 0;
}

static bool writeRsEncryptedHeader(FILE* file)
{
	if (fwrite("REDSHRT2", 9, 1, file) != 1)
		return false;

	const auto hashResultSize = HashResultSize();
	char hashResult[hashResultSize];
	memset(hashResult, 0, hashResultSize);
	const auto ret = fwrite(hashResult, hashResultSize, 1, file) == 1;
	return ret;
}

static bool writeRsEncryptedCheckSum(FILE* file)
{
	const auto hashResultSize = HashResultSize();
	byte data[hashResultSize];
	fseek(file, hashResultSize + 9, 0);
	if (RsFileCheckSum(file, data, hashResultSize) != hashResultSize)
		return false;
	
	fseek(file, 9, 0);
	return fwrite(data, hashResultSize, 1, file) == 1;
}

static bool noHeader(FILE* file)
{
	(void)file;
	return true;
}

static bool RsFileExists(const char* filePath)
{
	const auto file = fopen(filePath, "r");

	if (!file)
		return false;

	fclose(file);
	return true;
}

static void RsDeleteDirectory(char* filePath)
{
	rmdir(filePath);
}

static const char* RsBasename(const char* filePath)
{
	const auto lastSlash = strrchr(filePath, '/');
	const auto lastBackSlash = strrchr(filePath, '\\');

	const auto last = lastSlash > lastBackSlash ? lastSlash : lastBackSlash;

	if (last == nullptr)
		return filePath;

	filePath = last + 1;
	return filePath;
}

void RsCleanUp()
{
	char buffer[256];

	if (!gRsInitialised)
		return;

	gRsInitialised = 0;

	const auto dir = opendir(gRsTempDir);
	if (dir)
	{
		auto dirEntry = readdir(dir);

		while (dirEntry)
		{
			sprintf(buffer, "%s%s", gRsTempDir, dirEntry->d_name);
			remove(buffer);
			dirEntry = readdir(dir);
		}

		closedir(dir);
	}

	RsDeleteDirectory(gRsTempDir);
	BglCloseAllFiles();
}

bool RsFileEncrypted(const char* filePath)
{
	char headerBuffer[9];

	const auto file = fopen(filePath, "rb");

	if (!file)
		return false;

	if (fread(headerBuffer, 9, 1, file) != 1)
	{
		fclose(file);
		return false;
	}

	if (strcmp(headerBuffer, "REDSHIRT") == 0)
	{
		fclose(file);
		return true;
	}

	if (strcmp(headerBuffer, "REDSHRT2") == 0)
	{
		const auto hashResultSize = HashResultSize();
		auto testData = new char[hashResultSize];

		auto hashEqual = false;

		if (fread(testData, hashResultSize, 1, file) == 1)
		{
			auto hashResult = new byte[hashResultSize];
			auto checksumSize = RsFileCheckSum(file, hashResult, hashResultSize);

			if (checksumSize != 0)
				hashEqual = strncmp(testData, (const char*)hashResult, checksumSize) == 0;

			delete[] hashResult;
		}

		delete[] testData;

		fclose(file);
		return hashEqual;
	}

	fclose(file);
	return false;
}

bool RsFileEncryptedNoVerify(const char* filePath)
{
	const auto file = fopen(filePath, "rb");
	if (!file)
		return false;

	const auto ret = readRsEncryptedHeader(file);
	fclose(file);
	return ret;
}

FILE* RsFileOpen(const char* filePath, const char* mode)
{
	char tempFilePath[256];

	if (!RsFileExists(filePath))
		return nullptr;

	if (!RsFileEncrypted(filePath))
		return fopen(filePath, mode);

	sprintf(tempFilePath, "%s%s.d", gRsTempDir, RsBasename(filePath));

	if (filterFile(filePath, tempFilePath, readRsEncryptedHeader, noHeader, noHeader, decryptBuffer))
		return fopen(tempFilePath, mode);

	puts("Redshirt ERROR : Failed to write to output file");
	return nullptr;
}

bool RsEncryptFile(const char* filePath)
{
	return filterFileInPlace(filePath, ".e", noHeader, writeRsEncryptedHeader, writeRsEncryptedCheckSum, encryptBuffer);
}

void RsFileClose(const char* fileName, FILE* file)
{
	char buffer[0x100];

	fclose(file);
	sprintf(buffer, "%s.d", fileName);
	remove(buffer);
}

bool RsLoadArchive(const char* fileName)
{
	char buffer[256];

	sprintf(buffer, "%s%s", gRsAppPath, fileName);
	auto file = RsFileOpen(buffer, "rb");
	if (!file)
	{
		auto appPathLength = strlen(gRsAppPath);

		if (appPathLength < 5)
			return false;

		auto ch1 = gRsAppPath[appPathLength - 5];
		auto str = gRsAppPath - 4;
		auto ch5 = gRsAppPath[appPathLength - 1];

		if ((ch1 != '/' && ch1 != '\\') || (ch5 != '/' && ch5 != '\\') || strncasecmp(str, "lib", 3) != 0)
			return false;

		buffer[appPathLength - 4] = 0;
		strcat(buffer, fileName);

		file = RsFileOpen(buffer, "rb");

		if (!file)
			return false;
	}

	const auto ret = BglOpenZipFile(file, gRsAppPath, fileName);
	RsFileClose(fileName, file);
	printf(ret ? "Successfully loaded data archive %s\n" : "Failed to load data archive %s\n", fileName);
	return ret;
}

void RsInitialise(const char* appPath)
{
	strcpy(gRsAppPath, appPath);
	strcpy(gRsTempDir, "/tmp/uplink-XXXXXX");

	if (!mkdtemp(gRsTempDir))
	{
		puts("Failed to make temporary directory");
		abort();
	}

	const auto tempDirLength = strlen(gRsTempDir);
	gRsTempDir[tempDirLength] = '/';
	gRsTempDir[tempDirLength + 1] = 0;
	atexit(RsCleanUp);
	gRsInitialised = true;
}
