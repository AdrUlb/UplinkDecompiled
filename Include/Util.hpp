#pragma once

#include <ExceptionHandling.hpp>
#include <UplinkObject.hpp>
#include <csignal>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>

void PrintStackTrace();
bool DoesFileExist(const char* path);
void MakeDirectory(const char* path);
char* GetFilePath(const char* path);
void EmptyDirectory(const char* path);
void DeleteDirectory(const char* path);
bool FileExists(const char* path);
const char* Basename(const char* path);
bool FileReadDataImpl(const char* sourceFile, const int sourceLine, void* buffer, size_t size, size_t count, FILE* file);
bool LoadDynamicStringImpl(const char* sourceFile, const int sourceLine, char*& buffer, FILE* file);
void SaveDynamicString(const char* value, int maxSize, FILE* file);
void SaveDynamicString(const char* value, FILE* file);
UplinkObject* CreateUplinkObject(UplinkObjectId objectId);
void SetWindowScaleFactor(float x, float y);
int GetScaledXPosition(int pos);
int GetScaledYPosition(int pos);

template <class... Args>
static inline int UplinkSnprintfImpl(const char* file, const size_t line, char* s, size_t n, const char* format, Args... args)
{
	const auto ret = snprintf(s, n, format, args...);
	if (ret < 0 || static_cast<size_t>(ret) >= n)
	{
		printf("\n"
			   "An Uplink snprintf Failure has occured\n"
			   "======================================\n"
			   " Location    : %s, line %zu\n Buffer size : %zu\n Format      : %s\n Buffer      : %s\n",
			   file, line, n, format, s);
		__builtin_trap();
	}
	return ret;
}

static inline char* UplinkStrncpyImpl(const char* file, const size_t line, char* dest, const char* source, const size_t num)
{
	if (dest == source)
		return dest;

	const auto sourceSize = strlen(source);

	if (sourceSize >= num)
	{
		printf("\nAn Uplink strncpy Failure has occured\n"
			   "=====================================\n"
			   " Location    : %s, line %zu\n Dest. size  : %zu\n Source size : %zu\n Str. Source : %s\n",
			   file, line, num, sourceSize, source);
		__builtin_trap();
	}

	return strncpy(dest, source, num);
}

static inline void UplinkAssertImpl(const char* file, const size_t line, const char* condStr, bool cond)
{
	if (!cond)
	{
		printf("\n"
			   "An Uplink Assertion Failure has occured\n"
			   "=======================================\n"
			   " Condition : %s\n Location  : %s, line %zu\n",
			   condStr, file, line);
		__builtin_trap();
	}
}

[[noreturn]] static void inline UplinkAbortImpl(const char* file, const size_t line, const char* message)
{
	printf("\n"
		   "Uplink has been forced to Abort\n"
		   "===============================\n"
		   " Message   : %s\n Location  : %s, line %zu\n",
		   message, file, line);
	hSignalSIGSEGV(SIGSEGV);
	__builtin_trap();
}

#define UplinkSnprintf(s, n, format, ...) UplinkSnprintfImpl(__FILE__, __LINE__, (s), (n), (format), __VA_ARGS__)
#define UplinkStrncpy(dest, source, num) UplinkStrncpyImpl(__FILE__, __LINE__, (dest), (source), (num))
#define UplinkAssert(cond) UplinkAssertImpl(__FILE__, __LINE__, #cond, (cond))
#define UplinkAbort(message) UplinkAbortImpl(__FILE__, __LINE__, (message))
#define FileReadData(buffer, size, count, file) FileReadDataImpl(__FILE__, __LINE__, buffer, size, count, file)
#define LoadDynamicString(buffer, file) LoadDynamicStringImpl(__FILE__, __LINE__, buffer, file)
