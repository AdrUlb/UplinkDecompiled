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
