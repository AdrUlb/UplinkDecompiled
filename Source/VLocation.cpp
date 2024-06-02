#include <VLocation.hpp>

#include <Globals.hpp>
#include <Util.hpp>

bool VLocation::Load(FILE* file)
{
	if (!LoadDynamicStringBuf(ip, 0x18, file))
		return false;

	if (!LoadDynamicStringBuf(computer, 0x40, file))
		return false;

	if (!FileReadData(&x, 4, 1, file))
		return false;

	if (!FileReadData(&y, 4, 1, file))
		return false;

	if (!FileReadData(&listed, 1, 1, file))
		return false;

	if (!FileReadData(&displayed, 1, 1, file))
		return false;

	if (strcmp(game->GetLoadedSavefileVer(), "SAV60") >= 0)
		if (!FileReadData(&colored, 1, 1, file))
			return false;

	return true;
}

void VLocation::Save(FILE* file)
{
	SaveDynamicString(ip, 0x18, file);
	SaveDynamicString(computer, 0x40, file);
	fwrite(&x, 4, 1, file);
	fwrite(&y, 4, 1, file);
	fwrite(&listed, 1, 1, file);
	fwrite(&displayed, 1, 1, file);
	fwrite(&colored, 1, 1, file);
}

void VLocation::Print()
{
	printf("VLocation : ");
	printf("\tIP = %s, Computer = %s, x = %d, y = %d\n", ip, computer, x, y);
	printf("\tlisted = %d, displayed = %d, colored = %d\n", listed, displayed, colored);
}

const char* VLocation::GetID()
{
	return "VIRTLOC";
}

UplinkObjectId VLocation::GetOBJECTID()
{
	return UplinkObjectId::VLocation;
}

void VLocation::SetIP(const char* value)
{
	UplinkStrncpy(ip, value, 0x18);
}

void VLocation::SetPLocation(int x, int y)
{
	UplinkAssert(x <= 593);
	UplinkAssert(y <= 314);

	this->x = x;
	this->y = y;
}

void VLocation::SetComputer(const char* computerName)
{
	UplinkStrncpy(this->computer, computerName, 0x40);
}

void VLocation::SetListed(bool value)
{
	listed = value;
}

Computer* VLocation::GetComputer()
{
	auto tree = &game->GetWorld()->computers;

	while (true)
	{
		const auto computerTree = tree->LookupTree(this->computer);
		if (computerTree == nullptr)
			break;

		const auto comp = computerTree->Data;
		if (comp != nullptr)
		{
			if (strcmp(comp->GetIp(), ip) == 0)
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
