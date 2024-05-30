#include <Util.hpp>

#include <Globals.hpp>
#include <Options.hpp>
#include <RedShirt.hpp>
#include <cerrno>
#include <cstdio>
#include <dirent.h>
#include <execinfo.h>
#include <sys/stat.h>
#include <unistd.h>

static float windowScaleX = 1.0f;
static float windowScaleY = 1.0f;

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

void MakeDirectory(const char* path)
{
	mkdir(path, 0700);
}

char* GetFilePath(const char* path)
{
	UplinkAssert(path != nullptr);
	const auto bufferSize = strlen(path) + 1;
	char* buffer = new char[bufferSize];

	if (bufferSize != 0)
	{
		UplinkStrncpy(buffer, path, bufferSize);
		buffer[bufferSize - 1] = 0;
	}

	// Find final slash in path before file name
	char* finalSlash = strrchr(buffer, '/');

	// If the slash was not found, try backslash
	if (finalSlash == nullptr)
		finalSlash = strrchr(buffer, '\\');

	// STILL no slash? just delete the buffer and return dotslash
	if (finalSlash == nullptr)
	{
		delete[] buffer;
		return strdup("./");
	}

	// Place a null terminator after the final slash and return the buffer
	finalSlash[1] = 0;
	return buffer;
}

void EmptyDirectory(const char* path)
{
	auto dirp = opendir(path);
	if (dirp == nullptr)
		return;

	char filePath[0x100];

	for (auto entry = readdir(dirp); entry != nullptr; entry = readdir(dirp))
	{
		if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0)
		{
			UplinkSnprintf(filePath, sizeof(filePath), "%s%s", path);
			unlink(filePath);
			entry = readdir(dirp);
		}
	}

	closedir(dirp);
}

void DeleteDirectory(const char* path)
{
	rmdir(path);
}

bool FileExists(const char* path)
{
	const auto file = fopen(path, "r");

	if (file)
	{
		fclose(file);
		return true;
	}

	return false;
}

const char* Basename(const char* path)
{
	while (true)
	{
		auto dirSepStart = strchr(path, '/');
		if (!dirSepStart)
		{
			dirSepStart = strchr(path, '\\');
			if (!dirSepStart)
				return path;
		}

		path = dirSepStart + 1;
	}
}

bool FileReadDataImpl(const char* sourceFile, const int sourceLine, void* buffer, size_t size, size_t count, FILE* file)
{
	const auto actualCount = fread(buffer, size, count, file);

	if (count != actualCount)
		printf("WARNING: FileReadDataInt, request read count is different then the actual read count, request=%zu, actual=%zu, errno=%d, "
			   "%s:%d\n",
			   count, actualCount, errno, sourceFile, sourceLine);

	return count == actualCount;
}

bool LoadDynamicStringImpl(const char* sourceFile, const int sourceLine, char*& buffer, FILE* file)
{
	buffer = nullptr;
	int32_t length;
	if (!FileReadData(&length, 4, 1, file))
		return false;

	if (length == -1)
		return true;

	if (length > 0x4000)
	{
		printf("WARNING: LoadDynamicString, size appears to be wrong, size=%d, %s:%d\n", length, sourceFile, sourceLine);
		return false;
	}

	buffer = new char[length + 1];
	if (!FileReadData(buffer, length, 1, file))
	{
		buffer[length] = 0;
		return false;
	}
	buffer[length] = 0;

	return true;
}

bool LoadDynamicStringBufImpl(const char* sourceFile, const int sourceLine, char* buffer, const int max, FILE* file)
{
	if (buffer == nullptr)
	{
		printf("Print Assert: %s ln %d : %s\n", __FILE__, __LINE__, "buffer != nullptr");
		return false;
	}

	if (max > 0)
		buffer[0] = 0;

	int length;
	if (!FileReadData(&length, 4, 1, file))
		return false;

	if (length == -1)
	{
		printf("Print Abort: %s ln %d : ", __FILE__, __LINE__);
		printf("WARNING: LoadDynamicString, empty string, %s:%d\n", sourceFile, sourceLine);
		putchar('\n');
		return false;
	}

	if (length > max)
	{
		printf("Print Abort: %s ln %d : ", __FILE__, __LINE__);
		printf("WARNING: LoadDynamicString, size > maxsize, size=%d, maxsize=%d, %s:%d\n", length, max, sourceFile, sourceLine);
		if (max <= 0)
			return false;

		if (!FileReadData(buffer, max, 1, file))
		{
			buffer[max - 1] = 0;
			return false;
		}

		buffer[max - 1] = 0;
		printf("Print Abort: %s ln %d : ", __FILE__, __LINE__);
		printf("WARNING: LoadDynamicString, possible string=%s, %s:%d\n", buffer, sourceFile, sourceLine);
		return false;
	}

	if (length < 0)
	{
		printf("Print Abort: %s ln %d : ", __FILE__, __LINE__);
		printf("WARNING: LoadDynamicString, size appears to be wrong, size=%d, %s:%d\n", length, sourceFile, sourceLine);
		return false;
	}

	if (FileReadData(buffer, length, 1, file) == 0)
	{
		buffer[length - 1] = 0;
		return false;
	}
	buffer[length - 1] = 0;

	return true;
}

void SaveDynamicString(const char* value, int maxSize, FILE* file)
{
	if (value == nullptr)
	{
		const auto data = -1;
		fwrite(&data, 4, 1, file);
		return;
	}

	auto actualMaxSize = maxSize;
	if (maxSize < 0 || maxSize > 0x4000)
		actualMaxSize = 0x4000;

	auto size = strlen(value) + 1;

	if (size > (size_t)actualMaxSize)
	{
		printf("Print Abort: %s ln %d : ", "app/serialise.cpp", 0x3e5);
		printf("WARNING: SaveDynamicString, size appears to be too long, size=%zu, maxsize=%d, absolute  maxsize=%d", size, maxSize, 0x4000);
		putchar('\n');
		size = actualMaxSize;
	}

	fwrite(&size, 4, 1, file);

	if (size > 1)
		fwrite(value, size - 1, 1, file);

	char local_11 = 0;
	fwrite(&local_11, sizeof(local_11), 1, file);
}

void SaveDynamicString(const char* value, FILE* file)
{
	SaveDynamicString(value, -1, file);
	return;
}

UplinkObject* CreateUplinkObject(UplinkObjectId objectId)
{
	switch (objectId)
	{
		case UplinkObjectId::Option:
			return static_cast<UplinkObject*>(new Option());
			break;
		default:
			printf("Print Abort: %s ln %d : ", __FILE__, __LINE__);
			printf("Unrecognised OBJECTID=%d", static_cast<int>(objectId));
			return nullptr;
	}
}

void SetWindowScaleFactor(float x, float y)
{
	windowScaleX = x;
	windowScaleY = y;
}

int GetScaledXPosition(int pos)
{
	return pos * windowScaleX;
}

int GetScaledYPosition(int pos)
{
	return pos * windowScaleY;
}

DArray<char*>* ListDirectory(const char* dir, const char* ext)
{
	const auto files = RsListArchive(dir, ext);
	char dirPath[0x100];
	UplinkSnprintf(dirPath, 0x100, "%s%s", app->path, dir);

	const auto dirp = opendir(dirPath);
	if (dirp != nullptr)
	{
		for (const auto i = readdir(dirp); i != nullptr;)
		{
			if (strstr(i->d_name, ext) == nullptr)
				continue;

			const auto filePath = new char[0x100];
			UplinkSnprintf(filePath, 0x100, "%s%s", dir, i->d_name);
			files->PutData(filePath);
		}

		closedir(dirp);
	}

	if (files->Size() <= 0)
		return files;

	for (auto i = 0; i < files->Size() - 1; i++)
	{
		if (!files->ValidIndex(i))
			continue;

		for (auto j = i + 1; j < files->Size(); j++)
		{
			if (!files->ValidIndex(j))
				continue;

			const auto path1 = files->GetData(i);
			const auto path2 = files->GetData(j);
			if (strcmp(path1, path2) != 0)
				continue;

			delete[] path2;

			files->RemoveData(j);
		}
	}

	return files;
}

bool LoadDArray(DArray<UplinkObject*>* array, FILE* file)
{
	if (array == nullptr)
		return false;

	int itemCount;
	if (!FileReadData(&itemCount, 4, 1, file))
		return true;

	if (itemCount > 0x40000)
	{
		printf("Print Abort: %s ln %d : ", __FILE__, __LINE__);
		printf("WARNING: LoadDArray, number of items appears to be wrong, size=%d\n", itemCount);
		return false;
	}

	array->SetSize(itemCount);

	if (itemCount <= 0)
		return true;

	int index;
	for (auto i = 0; i < itemCount; i++)
	{
		if (!FileReadData(&index, 4, 1, file))
			return false;

		if (index == -1)
			continue;

		if (index >= 0x40000)
		{
			printf("Print Abort: %s ln %d : ", __FILE__, __LINE__);
			printf("WARNING: LoadDArray, number of items appears to be wrong, index=%d\n", index);
			return false;
		}

		UplinkObjectId objectId;
		if (FileReadData(&objectId, 4, 1, file) == 0)
			break;

		if (objectId == UplinkObjectId::Unknown)
		{
			printf("Print Abort: %s ln %d :\n%s\n", __FILE__, __LINE__, "LoadDArray WARNING : OID_UNKNOWN as OBJECTID");
			return false;
		}

		const auto obj = CreateUplinkObject(objectId);
		if (obj == nullptr)
			break;

		if (!obj->Load(file))
		{
			delete obj;
			break;
		}

		array->PutData(obj, index);
	}

	return true;
}

void SaveDArray(DArray<UplinkObject*>* array, FILE* file)
{
	UplinkAssert(array != nullptr);

	int32_t itemCount = array->Size();
	if (itemCount > 0x40000)
	{
		printf("Print Abort: %s ln %d : ", __FILE__, __LINE__);
		printf("WARNING: SaveDArray, number of items appears to be too big, size=%d, maxsize=%d\n", itemCount, 0x40000);
		itemCount = 0x40000;
	}

	fwrite(&itemCount, 4, 1, file);
	if (itemCount <= 0)
		return;

	for (auto i = 0; i < itemCount; i++)
	{
		if (!array->ValidIndex(i))
		{
			const auto buf = -1;
			fwrite(&buf, 4, 1, file);
			continue;
		}

		const auto obj = array->GetData(i);
		if (obj == nullptr)
		{
			const auto buf = -1;
			fwrite(&buf, 4, 1, file);
			continue;
		}

		fwrite(&i, 4, 1, file);
		const auto objectId = obj->GetOBJECTID();
		UplinkAssert(objectId != UplinkObjectId::Unknown);
		fwrite(&objectId, 4, 1, file);
		obj->Save(file);
	}
}

void PrintDArray(struct DArray<UplinkObject*>* array)
{
	UplinkAssert(array != nullptr);

	for (int32_t i = 0; i < array->Size(); i++)
	{
		printf("Index = %d\n", i);

		if (!array->ValidIndex(i))
		{
			puts("Not a Valid Index");
			continue;
		}

		const auto data = array->GetData(i);
		
		if (data == nullptr)
		{
			puts("NULL");
			continue;
		}

		data->Print();
	}
}

bool LoadLList(LList<char*>* list, FILE* file)
{
	UplinkAssert(list != nullptr);

	int itemCount;
	if (!FileReadData(&itemCount, 4, 1, file))
		return false;

	if (itemCount > 0x40000)
	{
		printf("Print Abort: %s ln %d : ", __FILE__, __LINE__);
		printf("WARNING: LoadLList, number of items appears to be wrong, size=%d\n", itemCount);
		return false;
	}

	for (auto i = 0; i < itemCount; i++)
	{
		char* str = nullptr;

		if (!LoadDynamicString(str, file))
			return false;

		list->PutData(str);
	}

	return true;
}

bool LoadLList(LList<UplinkObject*>* list, FILE* file)
{
	UplinkAssert(list != nullptr);

	int itemCount;
	if (!FileReadData(&itemCount, 4, 1, file))
		return false;

	if (itemCount > 0x40000)
	{
		printf("Print Abort: %s ln %d : ", "app/serialise.cpp", 0x185);
		printf("WARNING: LoadLList, number of items appears to be wrong, size=%d", itemCount);
		putchar(0xa);
		return 0;
	}

	if (itemCount <= 0)
		return true;

	for (auto i = 0; i < itemCount; i++)
	{
		UplinkObjectId objectId;
		if (!FileReadData(&objectId, 4, 1, file))
			return false;

		const auto obj = CreateUplinkObject(objectId);

		if (obj == nullptr)
			return true;

		if (!obj->Load(file))
		{
			if (obj != 0)
				delete obj;

			return true;
		}

		list->PutData(obj);
	}

	return true;
}

void SaveLList(LList<char*>* list, FILE* file)
{
	UplinkAssert(list != nullptr);

	auto itemCount = list->Size();

	if (itemCount > 0x40000)
	{
		printf("Print Abort: %s ln %d : ", __FILE__, __LINE__);
		printf("WARNING: SaveLList, number of items appears to be too big, size=%d, maxsize=%d\n", itemCount, 0x40000);
		itemCount = 0x40000;
	}

	fwrite(&itemCount, 4, 1, file);

	for (int32_t i = 0; i < itemCount; i++)
		SaveDynamicString(list->GetData(i), file);
}

void SaveLList(LList<UplinkObject*>* list, FILE* file)
{
	UplinkAssert(list != nullptr);

	auto itemCount = list->Size();
	if (itemCount <= 0)
	{
		itemCount = 0;
		fwrite(&itemCount, 4, 1, file);
		return;
	}

	auto validItemCount = 0;
	for (auto i = 0; i < itemCount; i++)
	{
		if (list->GetData(i) != nullptr)
			validItemCount++;
	}

	if (validItemCount > 0x40000)
	{
		printf("Print Abort: %s ln %d : ", __FILE__, __LINE__);
		printf("WARNING: SaveLList, number of items appears to be too big, size=%d, maxsize=%d\n", validItemCount, 0x40000);
		validItemCount = 0x40000;
	}

	fwrite(&validItemCount, 4, 1, file);

	validItemCount = 0;
	for (auto i = 0; i < itemCount && validItemCount < 0x40000; i++, validItemCount++)
	{
		const auto obj = list->GetData(i);
		if (obj == nullptr)
			continue;

		const auto objectId = obj->GetOBJECTID();

		UplinkAssert(objectId != UplinkObjectId::Unknown);
		fwrite(&objectId, 4, 1, file);

		obj->Save(file);

		validItemCount++;
	}
}

void PrintLList(LList<char*>* list)
{
	UplinkAssert(list != 0);

	for (auto i = 0; i < list->Size();)
	{
		if (list->GetData(i) != 0)
			printf("Index = %d : %s\n", i, list->GetData(i));
		else
			printf("Index = %d : NULL\n", i);
		i++;
	}
}

void PrintLList(LList<UplinkObject*>* list)
{
	UplinkAssert(list != 0);

	for (int rbx = 0; true; rbx++)
	{
		if (rbx >= list->Size())
			break;

		printf("Index = %d\n", rbx);
		const auto obj = list->GetData(rbx);

		if (obj == nullptr)
		{
			puts("NULL");
			continue;
		}

		obj->Print();
	}
}

void DeleteLListData(LList<char*>* list)
{
	UplinkAssert(list != nullptr);

	for (auto i = 0; i < list->Size(); i++)
	{
		const auto data = list->GetData(i);
		if (data != nullptr)
			delete[] data;
	}
}

void DeleteLListData(LList<UplinkObject*>* list)
{
	UplinkAssert(list != nullptr);

	for (auto i = 0; i < list->Size(); i++)
	{
		const auto data = list->GetData(i);
		if (data != nullptr)
			delete data;
	}
}

void PrintBTree(BTree<UplinkObject*>* tree)
{
	UplinkAssert(tree != nullptr);

	const auto elements = tree->ConvertToDArray();
	const auto indices = tree->ConvertIndexToDArray();

	for (int index = 0; index < elements->Size(); index++)
	{
		if (elements->ValidIndex(index))
			continue;

		UplinkAssert(indices->ValidIndex(index));
		printf("Index = %s\n", indices->GetData(index));

		const auto element = elements->GetData(index);

		if (element == nullptr)
		{
			puts("NULL");
		}
		else
		{
			element->Print();
		}
	}

	delete elements;
	delete indices;
}

void PrintBTree(BTree<char*>* tree)
{
	UplinkAssert(tree != nullptr);

	const auto elements = tree->ConvertToDArray();
	const auto indices = tree->ConvertIndexToDArray();

	for (int index = 0; index < elements->Size(); index++)
	{
		if (!elements->ValidIndex(index))
			continue;

		UplinkAssert(indices->ValidIndex(index));
		printf("Index = %s\n", indices->GetData(index));

		const auto element = elements->GetData(index);

		if (element == nullptr)
		{
			puts("NULL");
		}
		else
		{
			puts(element);
		}
	}

	delete elements;
	delete indices;
}

bool LoadBTree(BTree<UplinkObject*>* tree, FILE* file)
{
	if (!tree)
	{
		printf("LoadBTree called with tree = nullptr at %s:%d\n", __FILE__, __LINE__);
		return false;
	}

	int nodeCount;
	if (!FileReadData(&nodeCount, 4, 1, file))
		return false;

	if (nodeCount > 0x40000)
	{
		printf("WARNING: LoadBTree, number of items appears to be wrong, size=%d\n", nodeCount);
		return false;
	}

	for (auto i = 0; i < nodeCount; i++)
	{
		char* key = nullptr;
		if (!LoadDynamicString(key, file))
			return false;

		if (!key)
		{
			printf("WARNING: LoadBTree NULL id");
			return false;
		}

		int id;
		if (!FileReadData(&id, 4, 1, file))
			return false;

		auto value = CreateUplinkObject((UplinkObjectId)id);
		if (!value)
		{
			delete[] key;
			return false;
		}

		if (!value->Load(file))
		{
			delete[] key;
			delete value;
			return false;
		}

		tree->PutData(key, value);

		if (key)
			delete[] key;
	}
	return true;
}

void SaveBTree(BTree<UplinkObject*>* tree, FILE* file)
{
	UplinkAssert(tree);

	const auto elements = tree->ConvertToDArray();
	const auto indices = tree->ConvertIndexToDArray();

	const auto elementCountTotal = elements->Size();
	auto elementCountValid = 0;

	if (elementCountTotal <= 0)
	{
		int count = 0;
		fwrite(&count, 4, 1, file);
		delete elements;
		delete indices;
		return;
	}

	for (auto i = 0; i < elementCountTotal; i++)
	{
		if (elements->ValidIndex(i))
			elementCountValid++;
	}

	if (elementCountValid > 0x40000)
	{
		printf("Print Abort: %s ln %d : ", __FILE__, __LINE__);
		printf("WARNING: SaveBTree, number of items appears to be too big, size=%d, maxsize=%d", elementCountValid, 0x40000);
		putchar('\n');
		elementCountValid = 0x40000;
	}

	fwrite(&elementCountValid, 4, 1, file);

	elementCountValid = 0;
	for (auto i = 0; i < elementCountTotal && elementCountValid < 0x40000; i++)
	{
		if (!elements->ValidIndex(i))
			continue;

		UplinkAssert(indices->ValidIndex(i));
		UplinkAssert(elements->GetData(i));

		SaveDynamicString(indices->GetData(i), file);

		const auto obj = elements->GetData(i);
		const auto buf = obj->GetOBJECTID();
		UplinkAssert(buf != UplinkObjectId::Unknown);
		fwrite(&buf, 4, 1, file);
		obj->Save(file);
		elementCountValid++;
	}
	delete elements;
	delete indices;
}

void UpdateBTree(BTree<UplinkObject*>* tree)
{
	UplinkAssert(tree != 0);

	const auto objs = tree->ConvertToDArray();

	for (auto i = 0; i < objs->Size(); i++)
	{
		if (!objs->ValidIndex(i))
			continue;

		const auto obj = objs->GetData(i);
		if (obj == nullptr)
			continue;

		obj->Update();
	}
	delete objs;
}
