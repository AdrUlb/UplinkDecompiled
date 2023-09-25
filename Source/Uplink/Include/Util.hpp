#pragma once

#include <cstddef>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <sys/stat.h>
#include <BTree.hpp>
#include <UplinkObject.hpp>
#include <Options.hpp>

#define str(s) #s

#define UplinkCrash() (*((int*)0) = 0)

#define UplinkAssert(condition) \
	UplinkAssertImpl(condition, str(condition), __FILE__, __LINE__); 

#define UplinkStrncpy(destination, source, num) \
	UplinkStrncpyImpl(destination, source, num, __FILE__, __LINE__);

#define UplinkSnprintf(destination, num, format, ...) \
	UplinkSnprintfImpl(destination, num, format, __FILE__, __LINE__, __VA_ARGS__)

#define UplinkAbort(message) \
	UplinkAbortImpl(message, __FILE__, __LINE__)

#define FileReadDataInt(buffer, size, n, file) \
	FileReadDataIntImpl(__FILE__, __LINE__, buffer, size, n, file)

void UplinkAssertImpl(bool condition, const char* conditionStr, const char* location, int line);
char* UplinkStrncpyImpl(char* destination, const char* source, size_t num, const char* location, int line);
void UplinkAbortImpl(const char* messsage, const char* location, int line);

template<typename... Args>
static inline int UplinkSnprintfImpl(char* destination, size_t num, const char* format, const char* location, int line, Args... args)
{
	const auto ret = (size_t)snprintf(destination, num, format, args...);

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

bool FileReadDataIntImpl(const char* location, int line, void* buffer, uint size, uint n, FILE* file);

bool LoadDynamicStringInt(char* location, int line, char** buffer, FILE* file);

template<typename T>
void DeleteBTreeData(BTree<T>* tree)
{
	UplinkAssert(tree);

	const auto arr = tree->ConvertToDArray();

	for (auto i = 0; i < arr->Size(); i++)
	{
		if (arr->ValidIndex(i))
		{
			const auto data = arr->GetData(i);
			if (data)
				delete data;
		}
	}

	delete arr;
}

bool LoadBTree(BTree<char*>* tree, FILE* file);
bool LoadBTree(BTree<UplinkObject*>* tree, FILE* file);

inline bool LoadBTree(BTree<Option*>* tree, FILE* file)
{
	return LoadBTree((BTree<UplinkObject*>*)tree, file);
}
