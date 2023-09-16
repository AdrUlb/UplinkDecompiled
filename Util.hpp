#pragma once

#include <cstddef>
#include <cstdio>
#include <cstring>
#include <cstdlib>

#define str(s) #s

#define UplinkAssert(condition) \
	UplinkAssertImpl(condition, str(condition), __FILE__, __LINE__); 

#define UplinkStrncpy(destination, source, num) \
	UplinkStrncpyImpl(destination, source, num, __FILE__, __LINE__);

void UplinkAssertImpl(bool condition, const char* conditionStr, const char* location, int line);
char* UplinkStrncpyImpl(char* destination, const char* source, size_t num, const char* location, int line);
char* GetFilePath(const char* fileName);
