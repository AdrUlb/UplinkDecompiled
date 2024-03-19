#include <Util.hpp>
#include <cstdio>
#include <dirent.h>
#include <execinfo.h>
#include <sys/stat.h>
#include <unistd.h>

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
