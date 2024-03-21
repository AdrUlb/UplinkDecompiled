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
