#include <GatewayDef.hpp>

#include <Util.hpp>
#include <cstring>

GatewayDef::~GatewayDef()
{
	for (auto i = 0; true; i++)
	{
		if (i >= cpuLocations.Size())
			break;

		if (cpuLocations.ValidIndex(i))
		{
			const auto data = cpuLocations.GetData(i);
			if (data != nullptr)
				delete data;
		}
	}

	for (auto i = 0; true; i++)
	{
		if (i >= memoryLocations.Size())
			break;

		if (memoryLocations.ValidIndex(i))
		{
			const auto data = memoryLocations.GetData(i);
			if (data != nullptr)
				delete data;
		}
	}

	for (auto i = 0; true; i++)
	{
		if (i >= securityLocations.Size())
			break;
		if (securityLocations.ValidIndex(i))
		{
			const auto data = securityLocations.GetData(i);
			if (data != nullptr)
				delete data;
		}
	}
}

bool GatewayDef::Load(FILE* file)
{
	if (!LoadDynamicStringBuf(name, 0x80, file))
		return false;

	if (!LoadDynamicStringBuf(description, 0x100, file))
		return false;

	if (!LoadDynamicStringBuf(filename, 0x100, file))
		return false;

	if (!LoadDynamicStringBuf(thumbnail, 0x100, file))
		return false;

	if (!FileReadData(&cost, 4, 1, file))
		return false;

	if (cost < 0)
		cost = 0;

	if (!FileReadData(&maxCpus, 4, 1, file))
		return false;

	if (!FileReadData(&maxMemory, 4, 1, file))
		return false;

	if (!FileReadData(&maxUpgrades, 4, 1, file))
		return false;

	if (!FileReadData(&maxSecurity, 4, 1, file))
		return false;

	if (!FileReadData(&bandwidth, 4, 1, file))
		return false;

	if (!FileReadData(&width, 4, 1, file))
		return false;

	if (!FileReadData(&height, 4, 1, file))
		return false;

	if (!LoadDArrayGatewayDefLocation(&cpuLocations, file))
		return false;

	if (!LoadDArrayGatewayDefLocation(&memoryLocations, file))
		return false;

	if (!LoadDArrayGatewayDefLocation(&securityLocations, file))
		return false;

	if (!FileReadData(&modemX, 4, 1, file))
		return false;

	if (!FileReadData(&modemY, 4, 1, file))
		return false;

	if (!FileReadData(&powerX, 4, 1, file))
		return false;

	if (!FileReadData(&powerY, 4, 1, file))
		return false;

	return true;
}

void GatewayDef::Save(FILE* file)
{
	SaveDynamicString(name, 0x80, file);
	SaveDynamicString(description, 0x100, file);
	SaveDynamicString(filename, 0x100, file);
	SaveDynamicString(thumbnail, 0x100, file);
	fwrite(&cost, 4, 1, file);
	fwrite(&maxCpus, 4, 1, file);
	fwrite(&maxMemory, 4, 1, file);
	fwrite(&maxUpgrades, 4, 1, file);
	fwrite(&maxSecurity, 4, 1, file);
	fwrite(&bandwidth, 4, 1, file);
	fwrite(&width, 4, 1, file);
	fwrite(&height, 4, 1, file);
	SaveDArrayGatewayDefLocation(&cpuLocations, file);
	SaveDArrayGatewayDefLocation(&memoryLocations, file);
	SaveDArrayGatewayDefLocation(&securityLocations, file);
	fwrite(&modemX, 4, 1, file);
	fwrite(&modemY, 4, 1, file);
	fwrite(&powerX, 4, 1, file);
	fwrite(&powerY, 4, 1, file);
}

const char* GatewayDef::GetID()
{
	return "GATEWAYDEF";
}

void GatewayDef::LoadGatewayDefinition(std::ifstream& stream)
{
	char temp[0x100];
	stream >> std::ws;
	stream.getline(name, 0x80, '\r');
	if ((temp[0] = stream.get()) != '\n')
		stream.rdbuf()->sputbackc(temp[0]);

	stream >> std::ws >> cost;
	stream >> std::ws >> maxCpus;
	stream >> std::ws >> maxMemory;
	stream >> std::ws >> maxUpgrades;
	stream >> std::ws >> maxSecurity;
	stream >> std::ws >> bandwidth;
	stream >> std::ws >> width;
	stream >> std::ws >> height;

	if (cost < 0)
		cost = 0;

	stream >> std::ws;
	stream.getline(description, 0x100, '\r');
	if ((temp[0] = stream.get()) != '\n')
		stream.rdbuf()->sputbackc(temp[0]);

	// TODO: actually fix this properly
	if (strstr(description,
			   "An embedded broad-band radio transmitter provides total anonymity for the user - you cannot be caught with this gateway.") != nullptr)
	{
		strncpy(description, "A good small all rounder.", 0x100);
	}

	if (maxCpus > 0)
	{
		cpuLocations.SetSize(maxCpus);
		stream >> std::ws >> temp;

		for (auto i = 0; i < maxCpus; i++)
		{
			const auto loc = new GatewayDefLocation();
			stream >> std::ws >> loc->x >> std::ws >> loc->y;
			cpuLocations.PutData(loc, i);
		}
	}

	if (maxMemory > 0)
	{
		memoryLocations.SetSize(maxMemory);
		stream >> std::ws >> temp;

		for (auto i = 0; i < maxMemory; i++)
		{
			const auto loc = new GatewayDefLocation();
			stream >> std::ws >> loc->x >> std::ws >> loc->y;
			memoryLocations.PutData(loc, i);
		}
	}

	if (maxSecurity > 0)
	{
		securityLocations.SetSize(maxSecurity);
		stream >> std::ws >> temp;

		for (auto i = 0; i < maxSecurity; i++)
		{
			const auto loc = new GatewayDefLocation();
			stream >> std::ws >> loc->x >> std::ws >> loc->y;
			securityLocations.PutData(loc, i);
		}
	}

	stream >> std::ws >> temp;
	stream >> std::ws >> modemX >> std::ws >> modemY;

	stream >> std::ws >> temp;
	stream >> std::ws >> powerX >> std::ws >> powerY;
}

void GatewayDef::SetFilename(const char* filename)
{
	UplinkStrncpy(this->filename, filename, 0x100);
}

void GatewayDef::SetThumbnail(const char* thumbnail)
{
	UplinkStrncpy(this->thumbnail, thumbnail, 0x100);
}

bool GatewayDef::VerifyCorrectness()
{
	if (maxCpus > 0 && maxMemory > 0 && bandwidth > 0)
		return true;

	printf("WARNING : GatewayDef::VerifyCorrectness, Gateway '%s' loaded from '%s' as invald values, "
		   "(maxcpus <= %d || maxmemory <= %d || bandwidth <= %d )\n",
		   name, filename, maxCpus, maxMemory, bandwidth);
	return false;
}

int GatewayDef::GatewayDefComparator(GatewayDef** a, GatewayDef** b)
{
	const auto left = *a;
	const auto right = *b;

	if (left->cost > right->cost)
		return 1;

	if (left->cost < right->cost)
		return -1;

	return strcmp(left->name, right->name);
}
