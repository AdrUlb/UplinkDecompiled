#include <Util.hpp>
#include <cstdio>
#include <execinfo.h>
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
