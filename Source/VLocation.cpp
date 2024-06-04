#include <VLocation.hpp>

#include <Globals.hpp>
#include <Util.hpp>

bool VLocation::Load(FILE* file)
{
	if (!LoadDynamicStringBuf(_ip, 0x18, file))
		return false;

	if (!LoadDynamicStringBuf(_computerName, 0x40, file))
		return false;

	if (!FileReadData(&_x, 4, 1, file))
		return false;

	if (!FileReadData(&_y, 4, 1, file))
		return false;

	if (!FileReadData(&_listed, 1, 1, file))
		return false;

	if (!FileReadData(&_displayed, 1, 1, file))
		return false;

	if (strcmp(game->GetLoadedSavefileVer(), "SAV60") >= 0)
		if (!FileReadData(&_colored, 1, 1, file))
			return false;

	return true;
}

void VLocation::Save(FILE* file)
{
	SaveDynamicString(_ip, 0x18, file);
	SaveDynamicString(_computerName, 0x40, file);
	fwrite(&_x, 4, 1, file);
	fwrite(&_y, 4, 1, file);
	fwrite(&_listed, 1, 1, file);
	fwrite(&_displayed, 1, 1, file);
	fwrite(&_colored, 1, 1, file);
}

void VLocation::Print()
{
	printf("VLocation : ");
	printf("\tIP = %s, Computer = %s, x = %d, y = %d\n", _ip, _computerName, _x, _y);
	printf("\tlisted = %d, displayed = %d, colored = %d\n", _listed, _displayed, _colored);
}

const char* VLocation::GetID()
{
	return "VIRTLOC";
}

UplinkObjectId VLocation::GetOBJECTID()
{
	return UplinkObjectId::VLocation;
}

const char* VLocation::GetIp()
{
	return _ip;
}

const char* VLocation::GetComputerName()
{
	return _computerName;
}

int VLocation::GetX()
{
	return _x;
}

int VLocation::GetY()
{
	return _y;
}

bool VLocation::GetListed()
{
	return _listed;
}

bool VLocation::GetDisplayed()
{
	return _displayed;
}

bool VLocation::GetColored()
{
	return _colored;
}

void VLocation::SetIp(const char* ip)
{
	UplinkStrncpy(_ip, ip, 0x18);
}

void VLocation::SetPLocation(int x, int y)
{
	UplinkAssert(x <= 593);
	UplinkAssert(y <= 314);

	_x = x;
	_y = y;
}

void VLocation::SetComputerName(const char* computerName)
{
	UplinkStrncpy(_computerName, computerName, 0x40);
}

void VLocation::SetListed(bool listed)
{
	_listed = listed;
}

void VLocation::SetDisplayed(bool displayed)
{
	_displayed = displayed;
}

void VLocation::SetColorDisplayed(bool colored)
{
	_colored = colored;
}

Computer* VLocation::GetComputer()
{
	auto tree = &game->GetWorld().GetComputers();

	while (true)
	{
		const auto computerTree = tree->LookupTree(this->_computerName);
		if (computerTree == nullptr)
			break;

		const auto comp = computerTree->Data;
		if (comp != nullptr)
		{
			if (strcmp(comp->GetIp(), _ip) == 0)
				return comp;

			if (comp->GetType() == 64 && GetOBJECTID() == UplinkObjectId::VlocationSpecial)
				return comp;
		}

		tree = computerTree->Left();
		if (tree == nullptr)
			break;
	}

	return nullptr;
}

bool VLocationSpecial::Load(FILE* file)
{
	if (!VLocation::Load(file))
		return false;

	if (!FileReadData(&_screenIndex, 4, 1, file))
		return false;

	if (!FileReadData(&_securitySystemIndex, 4, 1, file))
		return false;

	return true;
}

void VLocationSpecial::Save(FILE* file)
{
	VLocation::Save(file);
	fwrite(&_screenIndex, 4, 1, file);
	fwrite(&_securitySystemIndex, 4, 1, file);
}

void VLocationSpecial::Print()
{
	puts("VLocationSpecial:");
	printf("screenIndex = %d\n", _screenIndex);
	printf("securitySystemIndex = %d\n", _securitySystemIndex);
	VLocation::Print();
}

const char* VLocationSpecial::GetID()
{
	return "VLOCSPEC";
}

UplinkObjectId VLocationSpecial::GetOBJECTID()
{
	return UplinkObjectId::VlocationSpecial;
}

int VLocationSpecial::GetScreenIndex()
{
	return _screenIndex;
}

int VLocationSpecial::GetSecuritySystemIndex()
{
	return _securitySystemIndex;
}
