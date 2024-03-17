#include <Util.hpp>
#include <cstdio>
#include <unistd.h>
#include <execinfo.h>

void PrintStackTrace()
{
	constexpr size_t bufferSize = 0x14;
	void* array[bufferSize];
	backtrace_symbols_fd(array, backtrace(array, bufferSize), STDOUT_FILENO);
}
