#pragma once

#include <BTree.hpp>
#include <DArray.hpp>
#include <ExceptionHandling.hpp>
#include <GatewayDef.hpp>
#include <LList.hpp>
#include <UplinkObject.hpp>
#include <csignal>
#include <cstddef>
#include <cstdint>
#include <cstdio>
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
bool LoadDynamicStringBufImpl(const char* sourceFile, const int sourceLine, char* buffer, const int max, FILE* file);
void SaveDynamicString(const char* value, int maxSize, FILE* file);
void SaveDynamicString(const char* value, FILE* file);
UplinkObject* CreateUplinkObject(UplinkObjectId objectId);
void SetWindowScaleFactor(float x, float y);
int GetScaledXPosition(int pos);
int GetScaledYPosition(int pos);
char* LowerCaseString(const char* str);
bool CopyFilePlain(const char* sourceFilePath, const char* destFilePath);
DArray<char*>* ListDirectory(const char* dir, const char* ext);
bool LoadDArray(DArray<int>* array, FILE* file);
bool LoadDArray(DArray<UplinkObject*>* array, FILE* file);
void SaveDArray(DArray<int>* array, FILE* file);
void SaveDArray(DArray<UplinkObject*>* array, FILE* file);
bool LoadDArrayGatewayDefLocation(DArray<GatewayDefLocation*>* array, FILE* file);
void SaveDArrayGatewayDefLocation(DArray<GatewayDefLocation*>* array, FILE* file);
void PrintDArray(DArray<int>* array);
void PrintDArray(DArray<UplinkObject*>* array);
bool LoadLList(LList<char*>* list, FILE* file);
bool LoadLList(LList<UplinkObject*>* list, FILE* file);
void SaveLList(LList<char*>* list, FILE* file);
void SaveLList(LList<UplinkObject*>* list, FILE* file);
void PrintLList(LList<char*>* list);
void PrintLList(LList<UplinkObject*>* list);
void DeleteLListData(LList<char*>* list);
void DeleteLListData(LList<UplinkObject*>* list);
bool LoadBTree(BTree<char*>* tree, FILE* file);
bool LoadBTree(BTree<UplinkObject*>* tree, FILE* file);
void PrintBTree(BTree<char*>* tree);
void PrintBTree(BTree<UplinkObject*>* tree);
void SaveBTree(BTree<char*>* tree, FILE* file);
void SaveBTree(BTree<UplinkObject*>* tree, FILE* file);
void UpdateBTree(BTree<UplinkObject*>* tree);
void DeleteDArrayDataD(DArray<char*>* array);
void DeleteDArrayDataD(DArray<UplinkObject*>* array);
void DeleteBTreeData(BTree<UplinkObject*>* tree);
void DeleteBTreeData(BTree<char*>* tree);

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
		RunUplinkExceptionHandling();
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
		RunUplinkExceptionHandling();
	}

	const auto ret = strncpy(dest, source, num);
	dest[num - 1] = 0;
	return ret;
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
		RunUplinkExceptionHandling();
	}
}

template <typename... Args> [[noreturn]] static void inline UplinkAbortImpl(const char* file, const size_t line, const char* message, Args... args)
{
	printf("\n"
		   "Uplink has been forced to Abort\n"
		   "===============================\n"
		   " Message   : ");
	printf(message, args...);
	printf("\n Location  : %s, line %zu\n", file, line);
	RunUplinkExceptionHandling();
}

#define UplinkSnprintf(s, n, format, ...) UplinkSnprintfImpl(__FILE__, __LINE__, (s), (n), (format)__VA_OPT__(, ) __VA_ARGS__)
#define UplinkStrncpy(dest, source, num) UplinkStrncpyImpl(__FILE__, __LINE__, (dest), (source), (num))
#define UplinkAssert(cond) UplinkAssertImpl(__FILE__, __LINE__, #cond, (cond))
#define UplinkAbort(message, ...) UplinkAbortImpl(__FILE__, __LINE__, (message)__VA_OPT__(, ) __VA_ARGS__)
#define FileReadData(buffer, size, count, file) FileReadDataImpl(__FILE__, __LINE__, buffer, size, count, file)
#define LoadDynamicString(buffer, file) LoadDynamicStringImpl(__FILE__, __LINE__, buffer, file)
#define LoadDynamicStringBuf(buffer, max, file) LoadDynamicStringBufImpl(__FILE__, __LINE__, buffer, max, file)

#ifdef UPLINKDECOMPILEDPRELOADED
const auto Temp_LoginInterface_RetireAgentClick = (void (*)(void))0x0048fee0;
#endif
