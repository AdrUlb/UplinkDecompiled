#pragma once

#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <cstdlib>

void PrintStackTrace();
bool DoesFileExist(const char* path);
void MakeDirectory(const char* path);
char* GetFilePath(const char* path);
void EmptyDirectory(const char* path);

template <class... Args>
__attribute__((always_inline)) static inline int UplinkSnprintfImpl(const char* file, const size_t line, char* s, size_t n,
																	const char* format, Args... args)
{
	const auto ret = snprintf(s, n, format, args...);
	if (ret < 0 || static_cast<size_t>(ret) >= n)
	{
		printf("\n"
			   "An Uplink snprintf Failure has occured\n"
			   "======================================\n"
			   " Location    : %s, line %zu\n Buffer size : %zu\n Format      : %s\n Buffer      : %s\n",
			   file, line, n, format, s);
		*(uint32_t*)0 = 0;
	}
	return ret;
}

__attribute__((always_inline)) static inline char* UplinkStrncpyImpl(const char* file, const size_t line, char* dest, const char* source,
																	 const size_t num)
{
	const auto sourceSize = strlen(source);

	if (sourceSize >= num)
	{
		printf("\nAn Uplink strncpy Failure has occured\n"
			   "=====================================\n"
			   " Location    : %s, line %zu\n Dest. size  : %zu\n Source size : %zu\n Str. Source : %s\n",
			   file, line, num, sourceSize, source);
		*(uint32_t*)0 = 0;
	}

	return strncpy(dest, source, num);
}

__attribute__((always_inline)) static inline void UplinkAssertImpl(const char* file, const size_t line, const char* condStr, bool cond)
{
	if (!cond)
	{
		printf("\n"
			   "An Uplink Assertion Failure has occured\n"
			   "=======================================\n"
			   " Condition : %s\n Location  : %s, line %zu\n",
			   condStr, file, line);
		*(uint32_t*)0 = 0;
	}
}

__attribute__((always_inline)) [[noreturn]] static void inline UplinkAbortImpl(const char* file, const size_t line, const char* message)
{
	printf("\n"
		   "Uplink has been forced to Abort\n"
		   "===============================\n"
		   " Message   : %s\n Location  : %s, line %zu\n",
		   message, file, line);
	*(uint64_t*)0 = 0;
	abort();
}

#define UplinkSnprintf(s, n, format, ...) UplinkSnprintfImpl(__FILE__, __LINE__, (s), (n), (format), __VA_ARGS__)
#define UplinkStrncpy(dest, source, num) UplinkStrncpyImpl(__FILE__, __LINE__, (dest), (source), (num))
#define UplinkAssert(cond) UplinkAssertImpl(__FILE__, __LINE__, #cond, (cond))
#define UplinkAbort(message) UplinkAbortImpl(__FILE__, __LINE__, (message))
