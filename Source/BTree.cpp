#include <BTree.hpp>

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