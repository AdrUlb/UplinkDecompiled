#include "RedShirt.hpp"

#include <cstring>
#include <cstdlib>
#include <cstdio>
#include "../../unrar/rar.hpp"
#include "../../unrar/sha1.hpp"

static bool gRsInitialised = false;
static char gRsAppPath[256];
static char gRsTempDir[256];

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

		(*filterCallback)(buffer, bytesRead);

		bytesWritten = fwrite(buffer, 1, bytesRead, tempFile);
	} while (bytesWritten >= bytesRead);
	return false;
}

static bool filterFile(char* filePath, char* tempFilePath, FilterFileCallback prepareReadCallback,
	FilterFileCallback prepareWriteCallback, FilterFileCallback finishCallback, FilterCallback filterCallback)
{
	bool temp;
	FILE* file;
	FILE* tempFile;

	file = fopen(filePath, "rb");

	if (!file)
		return false;

	if (!(*prepareReadCallback)(file))
	{
		puts("redshirt: failed to read header!");
		fclose(file);
		return false;
	}

	tempFile = fopen(tempFilePath, "w+b");
	if (!tempFile)
	{
		fclose(file);
		return false;
	}

	if (!(*prepareWriteCallback)(tempFile))
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

	temp = (*finishCallback)(tempFile);
	if (!temp)
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
	{
		buffer[i] = buffer[i] + 0x80;
	}
}

static bool readRsEncryptedHeader(FILE* file)
{
	char buffer[9];

	if (fread(buffer, 9, 1, file) != 1)
		return false;

	if (strcmp(buffer, "REDSHRT2"))
	{
		auto hashResultSize = HashResultSize();
		auto hashBuffer = new char[hashResultSize];

		auto ret = fread(hashBuffer, hashResultSize, 1, file) == 1;

		delete[] hashBuffer;

		return ret;
	}

	return strcmp(buffer, "REDSHIRT");
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

__attribute__((regparm(3)))
static size_t RsFileCheckSum(_IO_FILE* file, byte* data, uint length)
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

	if (strcmp(headerBuffer, "REDSHIRT"))
	{
		fclose(file);
		return true;
	}

	if (strcmp(headerBuffer, "REDSHRT2"))
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
	while (true);
	{
		auto seg = strchr(filePath, L'/');
		if (!seg)
		{
			seg = strchr(filePath, L'\\');
			if (!seg)
				return filePath;
		}
		filePath = seg + 1;
	}
}

static FILE* RsFileOpen(char* filePath, char* mode)
{
	const char* baseName;
	FILE* file;
	char tempFilePath[256];

	if (!RsFileExists(filePath))
		return nullptr;

	if (!RsFileEncrypted(filePath))
		return fopen(filePath, mode);

	baseName = RsBasename(filePath);
	sprintf(tempFilePath, "%s%s.d", gRsTempDir, baseName);

	if (!filterFile(filePath, tempFilePath, readRsEncryptedHeader, noHeader, noHeader, decryptBuffer))
	{
		puts("Redshirt ERROR : Failed to write to output file");
		return nullptr;
	}

	file = fopen(tempFilePath, mode);

	return file;
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
	gRsTempDir[0] = '/';
	gRsTempDir[1] = 0;
	gRsInitialised = true;
}
