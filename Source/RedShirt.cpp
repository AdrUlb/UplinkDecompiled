#include <Bungle.hpp>
#include <RedShirt.hpp>
#include <Util.hpp>
#include <cstdio>
#include <cstdlib>
#include <dirent.h>
#include <openssl/evp.h>

static char rsapppath[0x100];
static char tempdir[0x100];
static char tempfilename[0x100];

static bool rsInitialised = false;

typedef bool (*ReadHeaderCallback)(FILE*);
typedef bool (*WriteHeaderCallback)(FILE*);
typedef bool (*WriteChecksumCallback)(FILE*);
typedef void (*WriteDataCallback)(unsigned char*, unsigned int);

static inline size_t HashResultSize()
{
	return 20;
}

static EVP_MD_CTX* HashInitial()
{
	const auto context = EVP_MD_CTX_new();

	EVP_DigestInit_ex(context, EVP_sha1(), nullptr);
	return context;
}

static void HashData(EVP_MD_CTX* context, unsigned char* data, unsigned int length)
{
	EVP_DigestUpdate(context, data, length);
}

static unsigned int HashFinal(EVP_MD_CTX* context, void* buffer, unsigned int size)
{
	unsigned char digest[20];

	unsigned int outlen;
	EVP_DigestFinal_ex(context, digest, &outlen);
	EVP_MD_CTX_free(context);

	// FIXME? the original game fucked something up with the endianness, this is replicating that
	for (auto ptr = digest; ptr < digest + 20; ptr += 4)
	{
		auto temp = ptr[0];
		ptr[0] = ptr[3];
		ptr[3] = temp;

		temp = ptr[1];
		ptr[1] = ptr[2];
		ptr[2] = temp;
	}

	size = std::min<unsigned int>(outlen, size);
	memcpy(buffer, digest, size);

	return size;
}

static bool noHeader(FILE* file)
{
	(void)file;
	return true;
}

static void decryptBuffer(unsigned char* buffer, unsigned int size)
{
	for (unsigned int i = 0; i < size; i++)
		buffer[i] += 0x80;
}

static void encryptBuffer(unsigned char* buffer, unsigned int size)
{
	for (unsigned int i = 0; i < size; i++)
		buffer[i] += 0x80;
}

static bool readRsEncryptedHeader(FILE* file)
{
	char signature[9];

	if (fread(signature, 9, 1, file) != 1)
		return false;

	if (strcmp(signature, "REDSHRT2") == 0)
	{
		const auto hashResultSize = HashResultSize();
		const auto hash = new char[hashResultSize];

		auto success = fread(hash, hashResultSize, 1, file) == 1;

		if (hash)
			delete[] hash;

		return success;
	}

	return strcmp(signature, "REDSHIRT") == 0;
}

static bool writeRsEncryptedHeader(FILE* file)
{
	if (fwrite("REDSHRT2", 9, 1, file) != 1)
		return false;

	char buf[20]{0};
	return fwrite(buf, sizeof(buf), 1, file) == 1;
}

static bool writeRsEncryptedCheckSum(FILE* file)
{
	fseek(file, 20 + 9, 0);

	char buf[20];
	if (RsFileCheckSum(file, buf, 20) != 20)
		return false;

	fseek(file, 9, 0);
	return fwrite(buf, 20, 1, file) == 1;
}

static bool filterStream(FILE* file, FILE* filteredFile, WriteDataCallback writeDataCallback)
{
	unsigned char readBuffer[0x4000];

	size_t readCount;
	size_t writeCount;

	do
	{
		readCount = fread(readBuffer, 1, sizeof(readBuffer), file);

		if (readCount == 0)
			return true;

		writeDataCallback(readBuffer, readCount);

		writeCount = fwrite(readBuffer, 1, readCount, filteredFile);
	} while (readCount <= writeCount);

	return false;
}

static bool filterFile(const char* path, const char* filteredPath, ReadHeaderCallback readHeaderCallback, WriteHeaderCallback writeHeaderCallback,
					   WriteChecksumCallback writeChecksumCallback, WriteDataCallback writeDataCallback)
{

	auto file = fopen(path, "rb");

	if (!file)
		return false;

	if (!readHeaderCallback(file))
	{
		printf("redshirt: failed to read header!");
		fclose(file);
		return false;
	}

	auto filteredFile = fopen(filteredPath, "w+b");

	if (!filteredFile)
	{
		fclose(file);
		return false;
	}

	if (!writeHeaderCallback(filteredFile))
	{
		printf("redshirt: failed to write header!");
		fclose(file);
		fclose(filteredFile);
		remove(filteredPath);
		return false;
	}

	if (!filterStream(file, filteredFile, writeDataCallback))
	{
		printf("redshirt: failed to write containning bytes!");
		fclose(file);
		fclose(filteredFile);
		remove(filteredPath);
		return false;
	}

	if (!writeChecksumCallback(filteredFile))
	{
		printf("redshirt: failed to write checksum!\n");
		fclose(file);
		fclose(filteredFile);
		remove(filteredPath);
		return false;
	}

	fclose(file);
	fclose(filteredFile);
	return true;
}
bool filterFileInPlace(const char* path, const char* filteredExtension, ReadHeaderCallback readHeaderCallback,
					   WriteHeaderCallback writeHeaderCallback, WriteChecksumCallback writeChecksumCallback, WriteDataCallback writeDataCallback)
{
	char filteredPath[0x100];
	sprintf(filteredPath, "%s%s", path, filteredExtension);

	if (filterFile(path, filteredPath, readHeaderCallback, writeHeaderCallback, writeChecksumCallback, writeDataCallback) == 0)
	{
		puts("Redshirt ERROR : Failed to write output file");
		return false;
	}

	remove(path);
	rename(filteredPath, path);
	return true;
}

void RsInitialise(const char* appPath)
{
	strcpy(rsapppath, appPath);
	strncpy(tempdir, "/tmp/uplink-XXXXXX", 19);

	if (mkdtemp(tempdir) == 0)
	{
		puts("Failed to make temporary directory");
		abort();
	}

	uint64_t tempdirLength = strlen(tempdir);

	rsInitialised = true;

	tempdir[tempdirLength] = '/';
	tempdir[tempdirLength + 1] = 0;

	atexit(RsCleanUp);
}

void RsCleanUp()
{
	if (!rsInitialised)
		return;

	rsInitialised = false;

	auto dir = opendir(tempdir);

	char buffer[0x200];
	if (dir)
	{
		for (auto dirEntry = readdir(dir); dirEntry; dirEntry = readdir(dir))
		{
			snprintf(buffer, sizeof(buffer), "%s%s", tempdir, dirEntry->d_name);
			remove(buffer);
		}

		closedir(dir);
	}

	DeleteDirectory(tempdir);
	BglCloseAllFiles();
}

unsigned int RsFileCheckSum(FILE* file, void* buffer, size_t size)
{
	unsigned char readBuffer[0x4000];

	const auto hashContext = HashInitial();

	size_t readCount;
	while (true)
	{
		readCount = fread(readBuffer, 1, sizeof(readBuffer), file);

		if (readCount == 0)
			break;

		HashData(hashContext, readBuffer, readCount);
	}

	return HashFinal(hashContext, buffer, size);
}

bool RsFileEncryptedNoVerify(const char* path)
{
	const auto file = fopen(path, "rb");

	if (!file)
		return false;

	const auto ret = readRsEncryptedHeader(file);

	fclose(file);

	return ret;
}

bool RsFileEncrypted(const char* path)
{
	char signature[0x10];

	const auto file = fopen(path, "rb");

	if (!file)
		return false;

	if (fread(signature, 9, 1, file) != 1)
	{
		fclose(file);
		return false;
	}

	// File is RedShirt 1
	if (strcmp(signature, "REDSHIRT") == 0)
	{
		fclose(file);
		return true;
	}

	// File is neither RedShirt 1 nor 2
	if (strcmp(signature, "REDSHRT2") != 0)
	{
		fclose(file);
		return false;
	}

	// File is RedShirt 2

	const auto hashResultSize = HashResultSize();
	const auto expectedHash = new unsigned char[hashResultSize];

	auto success = false;

	if (fread(expectedHash, hashResultSize, 1, file) == 1)
	{
		const auto actualHash = new unsigned char[hashResultSize];
		const auto hashByteCount = RsFileCheckSum(file, actualHash, hashResultSize);

		if (hashByteCount != 0)
			success = memcmp(expectedHash, actualHash, hashByteCount) == 0;

		if (actualHash)
			delete[] actualHash;
	}

	if (expectedHash)
		delete[] expectedHash;

	fclose(file);
	return success;
}

FILE* RsFileOpen(const char* path, const char* modes)
{
	char buffer[0x200];

	if (!FileExists(path))
		return nullptr;

	if (!RsFileEncrypted(path))
		return fopen(path, modes);

	sprintf(buffer, "%s%s.d", tempdir, Basename(path));

	if (!filterFile(path, buffer, readRsEncryptedHeader, noHeader, noHeader, decryptBuffer))
	{
		puts("Redshirt ERROR : Failed to write to output file");
		return nullptr;
	}

	return fopen(buffer, modes);
}

void RsFileClose(const char* filePath, FILE* file)
{
	char buffer[0x100];

	fclose(file);
	sprintf(buffer, "%s.d", filePath);
	remove(buffer);
}

bool RsLoadArchive(const char* name)
{
	char buffer[0x100];
	sprintf(buffer, "%s%s", rsapppath, name);

	auto file = RsFileOpen(buffer, "rb");

	if (!file)
	{
		auto len = strlen(rsapppath);

		if (len < 5)
			return false;

		const auto c1 = rsapppath[len - 5];
		const auto c2 = rsapppath[len - 4];
		const auto c3 = rsapppath[len - 3];
		const auto c4 = rsapppath[len - 2];
		const auto c5 = rsapppath[len - 1];

		if ((c1 != '\\' && c1 != '/') || (c2 != 'l' && c2 != 'L') || (c3 != 'i' && c3 != 'I') || (c4 != 'b' && c4 != 'B') ||
			(c5 != '\\' && c5 != '/'))
			return false;

		buffer[len - 4] = 0;
		strcat(buffer, name);

		file = RsFileOpen(buffer, "rb");

		if (!file)
			return false;
	}

	printf("Loading data archive %s...\n", name);
	const auto success = BglOpenZipFile(file, rsapppath, name);

	RsFileClose(name, file);

	if (!success)
	{
		puts("ERROR LOADING ARCHIVE!");
		return false;
	}

	return true;
}

const char* RsArchiveFileOpen(const char* filePath)
{
	char fullPath[0x100];
	sprintf(fullPath, "%s%s", rsapppath, filePath);
	if (RsFileExists(fullPath))
	{
		strcpy(tempfilename, fullPath);
		return tempfilename;
	}

	if (BglFileLoaded(fullPath) == 0)
	{
		printf("REDSHIRT : Failed to load file : %s\n", fullPath);
		return nullptr;
	}

	const auto extension = strrchr(fullPath, '.');
	assert(extension);

	bool success = false;
	char extractPath[0x200];
	for (auto i = 0; i < 3; i++)
	{
		sprintf(extractPath, "%stemp%d%s", tempdir, i, extension);
		success = BglExtractFile(fullPath, extractPath);
		if (success)
			break;
	}

	if (!success)
	{
		printf("REDSHIRT : Failed to load file : %s\n", fullPath);
		return nullptr;
	}

	strcpy(tempfilename, extractPath);
	return tempfilename;
}

FILE* RsArchiveFileOpen(const char* filePath, const char* mode)
{
	const auto newFileName = RsArchiveFileOpen(filePath);
	if (newFileName == nullptr)
		return nullptr;

	return fopen(newFileName, mode);
}

int RsArchiveFileClose(const char* filePath, FILE* file)
{
	if (file != nullptr)
		fclose(file);

	const auto extension = strrchr(filePath, '.');
	assert(extension);
	int ret;

	char extractFilePath[0x200];
	for (int i = 0; i != 3; i++)
	{
		sprintf(extractFilePath, "%stemp%d%s", tempdir, i, extension);
		ret = remove(extractFilePath);
	}
	return ret;
}

bool RsFileExists(const char* const path)
{
	const auto file = fopen(path, "r");
	if (file != nullptr)
	{
		fclose(file);
		return true;
	}

	return false;
}

bool RsEncryptFile(const char* path)
{
	return filterFileInPlace(path, ".e", noHeader, writeRsEncryptedHeader, writeRsEncryptedCheckSum, encryptBuffer);
}

DArray<char*>* RsListArchive(const char* dir, const char* ext)
{
	const auto files = BglListFiles(rsapppath, dir, ext);

	for (auto index = 0; index < files->Size(); index++)
	{
		if (!files->ValidIndex(index))
			continue;

		const auto filePath = files->GetData(index);
		const auto value = new char[strlen(filePath) - strlen(rsapppath) + 1];
		strcpy(value, filePath + strlen(rsapppath));
		files->PutData(value, index);
		printf("Putting %s", value);
	}
	return files;
}
