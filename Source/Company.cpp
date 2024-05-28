#include <Company.hpp>

#include <NumberGenerator.hpp>
#include <Util.hpp>

bool Company::Load(FILE* file)
{
	if (!LoadDynamicStringBuf(name, 0x40, file))
		return false;

	if (!LoadDynamicStringBuf(boss, 0x80, file))
		return false;

	if (!LoadDynamicStringBuf(admin, 0x80, file))
		return false;

	if (!FileReadData(&size, 4, 1, file))
		return false;

	if (!FileReadData(&type, 4, 1, file))
		return false;

	if (!FileReadData(&growth, 4, 1, file))
		return false;

	if (!FileReadData(&alignment, 4, 1, file))
		return false;

	for (auto i = 0; i < 12; i++)
		if (!FileReadData(&sharePrices[i], 4, 1, file))
			return false;

	if (FileReadData(&sharePriceLastMonth, 4, 1, file) == 0)
		return false;

	return true;
}

void Company::Save(FILE* file)
{
	SaveDynamicString(name, 0x40, file);
	SaveDynamicString(boss, 0x80, file);
	SaveDynamicString(admin, 0x80, file);

	fwrite(&size, 4, 1, file);
	fwrite(&type, 4, 1, file);
	fwrite(&growth, 4, 1, file);
	fwrite(&alignment, 4, 1, file);

	for (auto i = 0; i < 12; i++)
		fwrite(&sharePrices[i], 4, 1, file);

	fwrite(&this->sharePriceLastMonth, 4, 1, file);

	SaveID_END(file);
}

void Company::Print()
{
	printf("Company : %s\n", name);
	printf("\tSize=%d, type=%d, Growth=%d, Alignment=%d\n", size, type, growth, alignment);
	printf("Boss    : %s\n", boss);
	printf("Admin   : %s\n", admin);
	puts("Share history");

	for (int i = 0; i < 12; i++)
		printf("%d:%dc  ", i, sharePrices[i]);

	printf("Last month set : %d\n", sharePriceLastMonth);
}

const char* Company::GetID()
{
	return "COMPANY";
}

UplinkObjectId Company::GetOBJECTID()
{
	return UplinkObjectId::Company;
}

void Company::SetName(const char* name)
{
	UplinkStrncpy(this->name, name, 0x40);
}

void Company::Grow(int amount)
{
	puts("TODO: implement Company::Grow()");
}

void Company::VaryGrowth()
{
	growth = NumberGenerator::ApplyVariance(growth, 20);
}
