#include <Util.hpp>

#include <cerrno>
#include <dirent.h>
#include <unistd.h>
#include "../UplinkDecompiledTempDefs.hpp"

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

	UplinkCrash();
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

		UplinkCrash();
	}

	return strncpy(destination, source, num);
}

void UplinkAbortImpl(const char* messsage, const char* location, int line)
{
	printf("\n"
		"Uplink has been forced to Abort\n"
		"===============================\n"
		" Message   : %s\n"
		" Location  : %s, line %d\n",

		messsage, location, line
	);

	UplinkCrash();
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

	const auto dir = opendir(buffer);

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

bool FileReadDataIntImpl(const char* location, int line, void* buffer, uint size, uint n, FILE* file)
{
	const auto readCount = fread(buffer, size, n, file);
	if (n != readCount)
	{
		printf("Print Abort: %s ln %d : ", __FILE__, __LINE__);
		printf("WARNING: FileReadDataInt, request read count is different then the readed count, request =%d, readed=%d, errno=%d, %s:%d\n", n, readCount, errno, location, line);
	}
	return n == readCount;
}

bool LoadDynamicStringInt(char** outBuffer, FILE* file)
{
	char* buf;

	*outBuffer = nullptr;
	int length;
	if (!FileReadDataInt(&length, sizeof(length), 1, file))
		return false;

	if (length == -1)
		return false;

	UplinkAssert(length <= 0x4000);

	buf = new char[length + 1];
	buf[length] = 0;

	*outBuffer = buf;

	if (!FileReadDataInt(buf, length, 1, file))
		return false;

	return true;
}

static void SaveDynamicString(char* str, int maxLength, FILE* file)
{
	size_t len;
	int lenWithNull;
	int local_18;
	char local_11;
	int lenWithNull2;

	if (str == (char*)0x0) {
		lenWithNull = -1;
		fwrite(&lenWithNull, 4, 1, file);
	}
	else {
		local_18 = 0x4000;
		if ((0 < maxLength) && (local_18 = maxLength, 0x4000 < maxLength)) {
			local_18 = 0x4000;
		}
		len = strlen(str);
		lenWithNull = len + 1;
		if (local_18 < lenWithNull) {
			printf("Print Abort: %s ln %d : ", "app/serialise.cpp", 0x3e5);
			printf("WARNING: SaveDynamicString, size appears to be too long, size=%d, maxsize=%d, absolute  maxsize=%d"
				, lenWithNull, maxLength, 0x4000);
			putchar(L'\n');
			fwrite(&local_18, 4, 1, file);
			lenWithNull2 = local_18;
		}
		else {
			fwrite(&lenWithNull, 4, 1, file);
			lenWithNull2 = lenWithNull;
		}
		if (1 < lenWithNull2) {
			fwrite(str, lenWithNull2 - 1, 1, file);
		}
		local_11 = '\0';
		fwrite(&local_11, 1, 1, file);
	}
}

static void SaveDynamicString(char* str, FILE* file)
{
	SaveDynamicString(str, -1, file);
}

bool LoadBTree(BTree<char*>* tree, FILE* file)
{
	UplinkAssert(tree);

	int count;
	if (!FileReadDataInt(&count, 4, 1, file))
		return false;

	UplinkAssert(count <= 0x40000);

	for (auto i = 0; i < count; i++)
	{
		char* name = nullptr;
		if (!LoadDynamicStringInt(&name, file))
			return false;

		UplinkAssert(name != nullptr);

		char* value = nullptr;
		if (!LoadDynamicStringInt(&value, file))
		{
			if (name)
			{
				delete[] name;
				return false;
			}

			return false;
		}

		tree->PutData(name, &value);

		delete[] name;
	}

	return true;
}

bool LoadBTree(BTree<UplinkObject*>* tree, FILE* file)
{
	UplinkAssert(tree);

	int count;
	if (!FileReadDataInt(&count, 4, 1, file))
		return false;

	UplinkAssert(count <= 0x40000);

	for (auto i = 0; i < count; i++)
	{
		char* name = nullptr;
		if (!LoadDynamicStringInt(&name, file))
			return false;

		UplinkAssert(name != nullptr);

		int objectId;
		if (!FileReadDataInt(&objectId, 4, 1, file))
			return false;

		auto obj = CreateUplinkObject(objectId);

		if (!obj || !obj->Load(file))
		{
			if (name)
				delete[] name;

			if (obj)
			{
				delete obj;
				return false;
			}
			return false;
		}

		tree->PutData(name, &obj);

		delete[] name;
	}

	return true;
}

void SaveBTree(BTree<char*>* tree, FILE* file)
{
	char cVar1;
	DArray<char*>* arr1;
	DArray<char*>* arr2;
	int iVar2;
	char* pcVar3;
	int iVar4;
	int local_14;

	UplinkAssert(tree);
	arr1 = tree->ConvertToDArray();
	arr2 = tree->ConvertIndexToDArray();
	iVar2 = arr1->Size();

	if (iVar2 < 1)
	{
		const auto val = 0;
		fwrite(&val, sizeof(val), 1, file);
	}
	else
	{
		local_14 = 0;
		iVar4 = 0;
		do
		{
			cVar1 = arr1->ValidIndex(iVar4);
			if (cVar1 != '\0') {
				local_14 = local_14 + 1;
			}
			iVar4 = iVar4 + 1;
		} while (iVar4 != iVar2);
		if (0x40000 < local_14)
		{
			printf("Print Abort: %s ln %d : ", "app/serialise.cpp", 0xe9);
			printf("WARNING: SaveBTree, number of items appears to be too big, size=%d, maxsize=%d",
				local_14, 0x40000);
			putchar(10);
			local_14 = 0x40000;
		}
		iVar4 = 0;
		fwrite(&local_14, 4, 1, file);
		local_14 = 0;
		do {
			cVar1 = arr1->ValidIndex(iVar4);
			if (cVar1 != '\0')
			{
				cVar1 = arr2->ValidIndex(iVar4);
				UplinkAssert(cVar1);
				pcVar3 = arr2->GetData(iVar4);
				SaveDynamicString(pcVar3, file);
				pcVar3 = arr1->GetData(iVar4);
				SaveDynamicString(pcVar3, file);
				local_14 = local_14 + 1;
			}
		} while ((iVar4 + 1 != iVar2) && (iVar4 = iVar4 + 1, local_14 < 0x40000));
	}

	delete arr1;
	delete arr2;
}

void SaveBTree(BTree<UplinkObject*>* tree, FILE* file)
{
	char cVar1;
	DArray<UplinkObject*>* arr1;
	DArray<char*>* arr2;
	int iVar2;
	UplinkObject* pUVar3;
	char* pcVar4;
	int iVar5;
	int local_18;
	int local_14;

	UplinkAssert(tree);
	arr1 = tree->ConvertToDArray();
	arr2 = tree->ConvertIndexToDArray();
	iVar2 = arr1->Size();

	local_14 = 0;
	if (iVar2 < 1)
	{
		fwrite(&local_14, sizeof(local_14), 1, file);
		local_14 = 0;
	}
	else
	{
		iVar5 = 0;
		do {
			cVar1 = arr1->ValidIndex(iVar5);
			if (cVar1 != '\0') {
				local_14 = local_14 + 1;
			}
			iVar5 = iVar5 + 1;
		} while (iVar5 != iVar2);
		if (0x40000 < local_14) {
			printf("Print Abort: %s ln %d : ", "app/serialise.cpp", 0x4f);
			printf("WARNING: SaveBTree, number of items appears to be too big, size=%d, maxsize=%d",
				local_14, 0x40000);
			putchar(10);
			local_14 = 0x40000;
		}
		iVar5 = 0;
		fwrite(&local_14, 4, 1, file);
		local_14 = 0;
		do {
			cVar1 = arr1->ValidIndex(iVar5);
			if (cVar1 != '\0') {
				cVar1 = arr2->ValidIndex(iVar5);
				UplinkAssert(cVar1);
				pUVar3 = arr1->GetData(iVar5);
				UplinkAssert(pUVar3);
				pcVar4 = arr2->GetData(iVar5);
				SaveDynamicString(pcVar4, file);
				pUVar3 = arr1->GetData(iVar5);
				local_18 = pUVar3->GetOBJECTID();
				UplinkAssert(local_18);
				fwrite(&local_18, 4, 1, file);
				pUVar3 = arr1->GetData(iVar5);
				pUVar3->Save(file);
				local_14 = local_14 + 1;
			}
		} while ((iVar5 + 1 != iVar2) && (iVar5 = iVar5 + 1, local_14 < 0x40000));
	}

	delete arr1;
	delete arr2;
}
