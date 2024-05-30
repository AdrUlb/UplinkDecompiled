#include <Company.hpp>

#include <Globals.hpp>
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

void Company::Grow(int amount)
{
	size *= ((amount / 90.0 * this->growth) + 100.0) / 100.0;

	auto month = game->GetWorld()->currentDate.GetMonth();

	UplinkAssert(month >= 1 && month <= 12);

	month--;
	sharePriceLastMonth = month;

	auto price = NumberGenerator::ApplyVariance(size, 30);
	if (price <= 0)
		price = 1;

	sharePrices[month] = price;

	while (true)
	{
		month--;
		if (month < 0)
			month = 11;

		if (sharePrices[month] != 0)
			break;

		sharePrices[month] = NumberGenerator::RandomNumber(size);
	}
}

void Company::VaryGrowth()
{
	growth = NumberGenerator::ApplyVariance(growth, 20);
}

void Company::SetName(const char* name)
{
	UplinkStrncpy(this->name, name, 0x40);
}

void Company::SetSize(int value)
{
	size = value;
}

void Company::SetTYPE(int value)
{
	type = value;
}

void Company::SetGrowth(int value)
{
	growth = value;
}

void Company::SetAlignment(int value)
{
	alignment = value;
}

CompanyUplink::CompanyUplink()
{
	SetName("Uplink");
}

CompanyUplink::~CompanyUplink()
{
	DeleteLListData(reinterpret_cast<LList<UplinkObject*>*>(&missions));
	DeleteLListData(reinterpret_cast<LList<UplinkObject*>*>(&softwareSales));
	DeleteLListData(reinterpret_cast<LList<UplinkObject*>*>(&hardwareSales));
	DeleteLListData(reinterpret_cast<LList<UplinkObject*>*>(&news));
}

bool CompanyUplink::Load(FILE* file)
{
	if (!Company::Load(file))
		return false;

	if (!LoadLList(reinterpret_cast<LList<UplinkObject*>*>(&missions), file))
		return false;

	if (!LoadLList(reinterpret_cast<LList<UplinkObject*>*>(&hardwareSales), file))
		return false;

	if (!LoadLList(reinterpret_cast<LList<UplinkObject*>*>(&softwareSales), file))
		return false;

	if (!LoadLList(reinterpret_cast<LList<UplinkObject*>*>(&news), file))
		return false;

	return true;
}

void CompanyUplink::Save(FILE* file)
{
	Company::Save(file);
	SaveLList(reinterpret_cast<LList<UplinkObject*>*>(&missions), file);
	SaveLList(reinterpret_cast<LList<UplinkObject*>*>(&hardwareSales), file);
	SaveLList(reinterpret_cast<LList<UplinkObject*>*>(&softwareSales), file);
	SaveLList(reinterpret_cast<LList<UplinkObject*>*>(&news), file);
}

void CompanyUplink::Print()
{
	Company::Print();
	PrintLList(reinterpret_cast<LList<UplinkObject*>*>(&missions));
	PrintLList(reinterpret_cast<LList<UplinkObject*>*>(&hardwareSales));
	PrintLList(reinterpret_cast<LList<UplinkObject*>*>(&softwareSales));
	PrintLList(reinterpret_cast<LList<UplinkObject*>*>(&news));
}

const char* CompanyUplink::GetID()
{
	return "CPNY_UPL";
}

UplinkObjectId CompanyUplink::GetOBJECTID()
{
	return UplinkObjectId::CompanyUplink;
}
