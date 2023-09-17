#include "RedShirt.hpp"

#include <cstring>
#include <cstdlib>
#include <cstdio>
#include "Bungle.hpp"
#include "../../unrar/rar.hpp"
#include "../../unrar/sha1.hpp"

// TODO: replace with own globals
//static bool gRsInitialised = false;
//static char gRsAppPath[256];
//static char gRsTempDir[256];
static auto gRsInitialisedPtr = (bool*)0x082070A9;
static auto gRsAppPath = (char*)0x08205DE0;
static auto gRsTempDir = (char*)0x08205EE0;
#define gRsInitialised (*gRsInitialisedPtr)

typedef void(*FilterCallback)(unsigned char* buffer, size_t size);
typedef bool(*FilterFileCallback)(FILE* file);

static inline int HashResultSize()
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
static bool filterStream(FILE* file, FILE* tempFile, FilterCallback filterCallback)
{
	size_t bytesRead;
	size_t bytesWritten;
	unsigned char buffer[0x4000];

	do
	{
		bytesRead = fread(buffer, 1, 0x4000, file);

		if (bytesRead == 0)
			return true;

		filterCallback(buffer, bytesRead);

		bytesWritten = fwrite(buffer, 1, bytesRead, tempFile);
	} while (bytesWritten >= bytesRead);
	return false;
}

static bool filterFile(char* filePath, char* tempFilePath, FilterFileCallback prepareReadCallback,
	FilterFileCallback prepareWriteCallback, FilterFileCallback finishCallback, FilterCallback filterCallback)
{
	auto file = fopen(filePath, "rb");

	if (!file)
		return false;

	if (!prepareReadCallback(file))
	{
		puts("redshirt: failed to read header!");
		fclose(file);
		return false;
	}

	auto tempFile = fopen(tempFilePath, "w+b");
	if (!tempFile)
	{
		fclose(file);
		return false;
	}

	if (!prepareWriteCallback(tempFile))
	{
		puts("redshirt: failed to write header!");
		fclose(file);
		fclose(tempFile);
		remove(tempFilePath);
		return false;
	}

	if (!filterStream(file, tempFile, filterCallback))
	{
		puts("redshirt: failed to write containning bytes!");
		fclose(file);
		fclose(tempFile);
		remove(tempFilePath);
		return false;
	}

	if (!finishCallback(tempFile))
	{
		puts("redshirt: failed to write checksum!");
		fclose(file);
		fclose(tempFile);
		remove(tempFilePath);
		return false;
	}

	fclose(file);
	fclose(tempFile);
	return true;
}

static void decryptBuffer(unsigned char* buffer, size_t size)
{
	for (size_t i = 0; i < size; i++)
		buffer[i] = buffer[i] + 0x80;
}

static bool readRsEncryptedHeader(FILE* file)
{
	char buffer[9];

	if (fread(buffer, 9, 1, file) != 1)
		return false;

	if (strcmp(buffer, "REDSHRT2") == 0)
	{
		auto hashResultSize = HashResultSize();
		auto hashResult = new char[hashResultSize];
		auto read = fread(hashResult, hashResultSize, 1, file);

		if (hashResult)
			delete[] hashResult;

		return read == 1;
	}

	return strcmp(buffer, "REDSHIRT") == 0;
}

static bool noHeader(FILE* file)
{
	return true;
}

static bool RsFileExists(char* filePath)
{
	auto file = fopen(filePath, "r");

	if (!file)
		return false;

	fclose(file);
	return true;
}

void RsDeleteDirectory(char* filePath)
{
	rmdir(filePath);
}

static size_t RsFileCheckSum(FILE* file, byte* data, uint length)
{
	size_t readCount;
	byte buffer[16384];

	auto context = HashInitial();
	while (true)
	{
		readCount = fread(buffer, 1, 0x4000, file);
		if (readCount == 0)
			break;
		HashData(context, buffer, readCount);
	}

	return HashFinal(context, data, length);
}

static bool RsFileEncrypted(char* filePath)
{
	char headerBuffer[9];

	auto file = fopen(filePath, "rb");

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
		auto hashResultSize = HashResultSize();
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

static const char* RsBasename(const char* filePath)
{
	auto lastSlash = strrchr(filePath, '/');
	auto lastBackSlash = strrchr(filePath, '\\');

	auto last = lastSlash > lastBackSlash ? lastSlash : lastBackSlash;
	filePath = last + 1;
	return filePath;
}

static FILE* RsFileOpen(char* filePath, const char* mode)
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

static void RsFileClose(char* fileName, FILE* file)
{
	char buffer[256];

	fclose(file);
	sprintf(buffer, "%s.d", fileName);
	remove(buffer);
	return;
}

static void RsCleanUp()
{
	dirent* dirEntry;
	char buffer[256];

	if (!gRsInitialised)
		return;

	gRsInitialised = 0;

	auto dir = opendir(gRsTempDir);
	if (dir)
	{
		dirEntry = readdir(dir);

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

bool RsLoadArchive(char* fileName)
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

		if ((ch1 != '/' && ch1 != '\\') || (ch5 != '/' && ch5 != '\\') || strncasecmp(gRsAppPath - 4, "lib", 3) != 0)
			return false;

		buffer[appPathLength - 4] = 0;
		strcat(buffer, fileName);

		file = RsFileOpen(buffer, "rb");

		if (!file)
			return false;
	}

	auto ret = BglOpenZipFile(file, gRsAppPath, fileName);
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

	auto tempDirLength = strlen(gRsTempDir);
	gRsTempDir[tempDirLength] = '/';
	gRsTempDir[tempDirLength + 1] = 0;
	atexit(RsCleanUp);
	gRsInitialised = true;
}
