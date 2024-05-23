#include <Options.hpp>
#include <Util.hpp>
#include <cerrno>
#include <cstdio>
#include <dirent.h>
#include <execinfo.h>
#include <sys/stat.h>
#include <unistd.h>

static float windowScaleX;
static float windowScaleY;

void PrintStackTrace()
{
	constexpr size_t bufferSize = 0x14;
	void* array[bufferSize];
	backtrace_symbols_fd(array, backtrace(array, bufferSize), STDOUT_FILENO);
}

bool DoesFileExist(const char* path)
{
	return access(path, F_OK) == 0;
}

void MakeDirectory(const char* path)
{
	mkdir(path, 0700);
}

char* GetFilePath(const char* path)
{
	UplinkAssert(path != nullptr);
	const auto bufferSize = strlen(path) + 1;
	char* buffer = new char[bufferSize];

	if (bufferSize != 0)
	{
		UplinkStrncpy(buffer, path, bufferSize);
		buffer[bufferSize - 1] = 0;
	}

	// Find final slash in path before file name
	char* finalSlash = strrchr(buffer, '/');

	// If the slash was not found, try backslash
	if (finalSlash == nullptr)
		finalSlash = strrchr(buffer, '\\');

	// STILL no slash? just delete the buffer and return dotslash
	if (finalSlash == nullptr)
	{
		delete[] buffer;
		return strdup("./");
	}

	// Place a null terminator after the final slash and return the buffer
	finalSlash[1] = 0;
	return buffer;
}

void EmptyDirectory(const char* path)
{
	auto dirp = opendir(path);
	if (dirp == nullptr)
		return;

	char filePath[0x100];

	for (auto entry = readdir(dirp); entry != nullptr; entry = readdir(dirp))
	{
		if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0)
		{
			UplinkSnprintf(filePath, sizeof(filePath), "%s%s", path);
			unlink(filePath);
			entry = readdir(dirp);
		}
	}

	closedir(dirp);
}

void DeleteDirectory(const char* path)
{
	rmdir(path);
}

bool FileExists(const char* path)
{
	const auto file = fopen(path, "r");

	if (file)
	{
		fclose(file);
		return true;
	}

	return false;
}

const char* Basename(const char* path)
{
	while (true)
	{
		auto dirSepStart = strchr(path, '/');
		if (!dirSepStart)
		{
			dirSepStart = strchr(path, '\\');
			if (!dirSepStart)
				return path;
		}

		path = dirSepStart + 1;
	}
}

bool FileReadDataImpl(const char* sourceFile, const int sourceLine, void* buffer, size_t size, size_t count, FILE* file)
{
	const auto actualCount = fread(buffer, size, count, file);

	if (count != actualCount)
		printf("WARNING: FileReadDataInt, request read count is different then the actual read count, request=%zu, actual=%zu, errno=%d, "
			   "%s:%d\n",
			   count, actualCount, errno, sourceFile, sourceLine);

	return count == actualCount;
}
bool LoadDynamicStringImpl(const char* sourceFile, const int sourceLine, char*& buffer, FILE* file)
{
	buffer = nullptr;
	int32_t length;
	if (!FileReadData(&length, sizeof(length), 1, file))
		return false;

	if (length == -1)
		return true;

	if (length > 0x4000)
	{
		printf("WARNING: LoadDynamicString, size appears to be wrong, size=%d, %s:%d\n", length, sourceFile, sourceLine);
		return false;
	}

	buffer = new char[length + 1];
	if (!FileReadData(buffer, length, 1, file))
	{
		buffer[length] = 0;
		return false;
	}
	buffer[length] = 0;

	return true;
}

void SaveDynamicString(const char* value, int maxSize, FILE* file)
{
	if (value == nullptr)
	{
		const auto data = -1;
		fwrite(&data, 4, 1, file);
		return;
	}

	auto actualMaxSize = maxSize;
	if (maxSize < 0 || maxSize > 0x4000)
		actualMaxSize = 0x4000;

	auto size = strlen(value) + 1;

	if (size > (size_t)actualMaxSize)
	{
		printf("Print Abort: %s ln %d : ", "app/serialise.cpp", 0x3e5);
		printf("WARNING: SaveDynamicString, size appears to be too long, size=%zu, maxsize=%d, absolute  maxsize=%d", size, maxSize,
			   0x4000);
		putchar('\n');
		size = actualMaxSize;
	}

	fwrite(&size, 4, 1, file);

	if (size > 1)
		fwrite(value, size - 1, 1, file);

	char local_11 = 0;
	fwrite(&local_11, sizeof(local_11), 1, file);
}

void SaveDynamicString(const char* value, FILE* file)
{
	SaveDynamicString(value, -1, file);
	return;
}

UplinkObject* CreateUplinkObject(UplinkObjectId objectId)
{
	switch (objectId)
	{
		case UplinkObjectId::Option:
			return static_cast<UplinkObject*>(new Option());
			break;
		default:
			printf("Print Abort: %s ln %d : ", __FILE__, __LINE__);
			printf("Unrecognised OBJECTID=%d", static_cast<int>(objectId));
			return nullptr;
	}
}

void SetWindowScaleFactor(float x, float y)
{
	windowScaleX = x;
	windowScaleY = y;
}

int GetScaledXPosition(int pos)
{
	return pos * windowScaleX;
}

int GetScaledYPosition(int pos)
{
	return pos * windowScaleY;
}
