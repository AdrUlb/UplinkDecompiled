#include <Company.hpp>

#include <Globals.hpp>
#include <NumberGenerator.hpp>
#include <Util.hpp>

bool Company::Load(FILE* file)
{
	if (!LoadDynamicStringBuf(_name, 0x40, file))
		return false;

	if (!LoadDynamicStringBuf(_boss, 0x80, file))
		return false;

	if (!LoadDynamicStringBuf(_admin, 0x80, file))
		return false;

	if (!FileReadData(&_size, 4, 1, file))
		return false;

	if (!FileReadData(&_type, 4, 1, file))
		return false;

	if (!FileReadData(&_growth, 4, 1, file))
		return false;

	if (!FileReadData(&_alignment, 4, 1, file))
		return false;

	for (auto i = 0; i < 12; i++)
		if (!FileReadData(&_sharePrices[i], 4, 1, file))
			return false;

	if (FileReadData(&_sharePriceLastMonth, 4, 1, file) == 0)
		return false;

	return true;
}

void Company::Save(FILE* file)
{
	SaveDynamicString(_name, 0x40, file);
	SaveDynamicString(_boss, 0x80, file);
	SaveDynamicString(_admin, 0x80, file);

	fwrite(&_size, 4, 1, file);
	fwrite(&_type, 4, 1, file);
	fwrite(&_growth, 4, 1, file);
	fwrite(&_alignment, 4, 1, file);

	for (auto i = 0; i < 12; i++)
		fwrite(&_sharePrices[i], 4, 1, file);

	fwrite(&this->_sharePriceLastMonth, 4, 1, file);
}

void Company::Print()
{
	printf("Company : %s\n", _name);
	printf("\tSize=%d, type=%d, Growth=%d, Alignment=%d\n", _size, _type, _growth, _alignment);
	printf("Boss    : %s\n", _boss);
	printf("Admin   : %s\n", _admin);
	puts("Share history");

	for (int i = 0; i < 12; i++)
		printf("%d:%dc  ", i, _sharePrices[i]);

	printf("Last month set : %d\n", _sharePriceLastMonth);
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
	_size *= ((amount / 90.0 * this->_growth) + 100.0) / 100.0;

	auto month = game->GetWorld().GetCurrentDate().GetMonth();

	UplinkAssert(month >= 1 && month <= 12);

	month--;
	_sharePriceLastMonth = month;

	auto price = NumberGenerator::ApplyVariance(_size, 30);
	if (price <= 0)
		price = 1;

	_sharePrices[month] = price;

	while (true)
	{
		month--;
		if (month < 0)
			month = 11;

		if (_sharePrices[month] != 0)
			break;

		_sharePrices[month] = NumberGenerator::RandomNumber(_size);
	}
}

void Company::VaryGrowth()
{
	_growth = NumberGenerator::ApplyVariance(_growth, 20);
}

const char* Company::GetName()
{
	return _name;
}

int Company::GetSize()
{
	return _size;
}

void Company::SetName(const char* name)
{
	UplinkStrncpy(this->_name, name, 0x40);
}

void Company::SetSize(int size)
{
	_size = size;
}

void Company::SetTYPE(int type)
{
	_type = type;
}

void Company::SetGrowth(int growth)
{
	_growth = growth;
}

void Company::SetAlignment(int alignment)
{
	_alignment = alignment;
}

CompanyUplink::CompanyUplink()
{
	SetName("Uplink");
}

CompanyUplink::~CompanyUplink()
{
	DeleteLListData(reinterpret_cast<LList<UplinkObject*>*>(&_missions));
	DeleteLListData(reinterpret_cast<LList<UplinkObject*>*>(&_softwareSales));
	DeleteLListData(reinterpret_cast<LList<UplinkObject*>*>(&_hardwareSales));
	DeleteLListData(reinterpret_cast<LList<UplinkObject*>*>(&_news));
}

bool CompanyUplink::Load(FILE* file)
{
	if (!Company::Load(file))
		return false;

	if (!LoadLList(reinterpret_cast<LList<UplinkObject*>*>(&_missions), file))
		return false;

	if (!LoadLList(reinterpret_cast<LList<UplinkObject*>*>(&_hardwareSales), file))
		return false;

	if (!LoadLList(reinterpret_cast<LList<UplinkObject*>*>(&_softwareSales), file))
		return false;

	if (!LoadLList(reinterpret_cast<LList<UplinkObject*>*>(&_news), file))
		return false;

	return true;
}

void CompanyUplink::Save(FILE* file)
{
	Company::Save(file);
	SaveLList(reinterpret_cast<LList<UplinkObject*>*>(&_missions), file);
	SaveLList(reinterpret_cast<LList<UplinkObject*>*>(&_hardwareSales), file);
	SaveLList(reinterpret_cast<LList<UplinkObject*>*>(&_softwareSales), file);
	SaveLList(reinterpret_cast<LList<UplinkObject*>*>(&_news), file);
}

void CompanyUplink::Print()
{
	Company::Print();
	PrintLList(reinterpret_cast<LList<UplinkObject*>*>(&_missions));
	PrintLList(reinterpret_cast<LList<UplinkObject*>*>(&_hardwareSales));
	PrintLList(reinterpret_cast<LList<UplinkObject*>*>(&_softwareSales));
	PrintLList(reinterpret_cast<LList<UplinkObject*>*>(&_news));
}

const char* CompanyUplink::GetID()
{
	return "CPNY_UPL";
}

UplinkObjectId CompanyUplink::GetOBJECTID()
{
	return UplinkObjectId::CompanyUplink;
}
