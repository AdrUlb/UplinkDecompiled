#include <LList.hpp>
#include <Util.hpp>

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
