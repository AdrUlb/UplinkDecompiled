#include "Util.hpp"

#include <dirent.h>
#include <unistd.h>

#define Crash() (*((int*)0) = 0)

void UplinkAssertImpl(bool condition, const char* conditionStr, const char* location, int line)
{
	if (condition)
		return;

	printf(
		"\n"
		"An Uplink Assertion Failure has occured\n"
		"=======================================\n"
		" Condition : %s\n"
		" Location  : %s, line %d\n",

		conditionStr, location, line
	);

	Crash();
}

char* UplinkStrncpyImpl(char* destination, const char* source, size_t num, const char* location, int line)
{
	auto sourceLength = strlen(source);

	if (sourceLength >= num)
	{
		printf(
			"\n"
			"An Uplink strncpy Failure has occured\n"
			"=====================================\n"
			" Location    : %s, line %d\n"
			" Dest. size  : %d\n"
			" Source size : %d\n"
			" Str. Source : %s\n",

			location, line, num, sourceLength, source
		);

		Crash();
	}

	return strncpy(destination, source, num);
}

template<typename... Args>
int UplinkSnprintfImpl(char* destination, size_t num, const char* format, const char* location, int line, Args... args)
{
	auto ret = snprintf(destination, num, format, args...);

	if (ret > num)
	{
		printf(
			"\n"
			"An Uplink snprintf Failure has occured\n"
			"======================================\n"
			" Location    : %s, line %d\n"
			" Buffer size : %d\n Format      : %s\n"
			" Buffer      : %s\n",

			location, line, num, format, destination
		);

		Crash();
	}

	return ret;
}

char* GetFilePath(const char* fileName)
{
	UplinkAssert(fileName);

	auto fileNameLength = strlen(fileName);
	auto filePathLength = fileNameLength + 1;
	auto filePath = new char[filePathLength];
	if (filePathLength != 0)
	{
		UplinkStrncpy(filePath, fileName, filePathLength);
		filePath[fileNameLength] = 0;
	}

	auto slashPtr = strrchr(filePath, '/');

	if (!slashPtr)
	{
		slashPtr = strrchr(filePath, '\\');
		if (!slashPtr)
		{
			if (filePath)
				delete[] filePath;

			auto dotSlashBuffer = new char[3];
			dotSlashBuffer[0] = '.';
			dotSlashBuffer[1] = '/';
			dotSlashBuffer[2] = 0;
			return dotSlashBuffer;
		}
	}

	slashPtr[1] = 0;

	return filePath;
}

void EmptyDirectory(const char* path)
{
	char buffer[PATH_MAX];

	UplinkStrncpy(buffer, path, 0x100);

	auto dir = opendir(buffer);

	if (!dir)
		return;

	for (auto dirEntry = readdir(dir); dirEntry; dirEntry = readdir(dir))
	{
		if (strncmp(dirEntry->d_name, ".", 2) == 0)
			continue;

		if (strncmp(dirEntry->d_name, "..", 3) == 0)
			continue;

		UplinkSnprintf(buffer, PATH_MAX, "%s%s", path, dirEntry->d_name);
		unlink(buffer);
	}
	closedir(dir);
}

bool DoesFileExist(const char* path)
{
	return !access(path, 0);
}
