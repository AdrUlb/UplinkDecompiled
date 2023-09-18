#pragma once

#include <cstddef>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <sys/stat.h>

#define str(s) #s

#define UplinkCrash() (*((int*)0) = 0)

#define UplinkAssert(condition) \
	UplinkAssertImpl(condition, str(condition), __FILE__, __LINE__); 

#define UplinkStrncpy(destination, source, num) \
	UplinkStrncpyImpl(destination, source, num, __FILE__, __LINE__);

#define UplinkSnprintf(destination, num, format, ...) \
	UplinkSnprintfImpl(destination, num, format, __FILE__, __LINE__, __VA_ARGS__)

void UplinkAssertImpl(bool condition, const char* conditionStr, const char* location, int line);
char* UplinkStrncpyImpl(char* destination, const char* source, size_t num, const char* location, int line);

template<typename... Args>
static int UplinkSnprintfImpl(char* destination, size_t num, const char* format, const char* location, int line, Args... args)
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

		UplinkCrash();
	}

	return ret;
}

char* GetFilePath(const char* fileName);

inline void MakeDirectory(const char* path)
{
	mkdir(path, 0700);
}

void EmptyDirectory(const char* path);
bool DoesFileExist(const char* path);
