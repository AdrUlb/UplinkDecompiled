#include <LList.hpp>
#include <Util.hpp>

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

bool LoadLList(struct LList<UplinkObject*>* list, FILE* file)
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
