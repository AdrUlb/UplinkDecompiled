#include <GatewayDef.hpp>

#include <Util.hpp>
#include <cstring>

GatewayDef::~GatewayDef()
{
	for (auto i = 0; true; i++)
	{
		if (i >= _cpuLocations.Size())
			break;

		if (_cpuLocations.ValidIndex(i))
		{
			const auto data = _cpuLocations.GetData(i);
			if (data != nullptr)
				delete data;
		}
	}

	for (auto i = 0; true; i++)
	{
		if (i >= _memoryLocations.Size())
			break;

		if (_memoryLocations.ValidIndex(i))
		{
			const auto data = _memoryLocations.GetData(i);
			if (data != nullptr)
				delete data;
		}
	}

	for (auto i = 0; true; i++)
	{
		if (i >= _securityLocations.Size())
			break;
		if (_securityLocations.ValidIndex(i))
		{
			const auto data = _securityLocations.GetData(i);
			if (data != nullptr)
				delete data;
		}
	}
}

bool GatewayDef::Load(FILE* file)
{
	if (!LoadDynamicStringBuf(_name, 0x80, file))
		return false;

	if (!LoadDynamicStringBuf(_description, 0x100, file))
		return false;

	if (!LoadDynamicStringBuf(_filename, 0x100, file))
		return false;

	if (!LoadDynamicStringBuf(_thumbnail, 0x100, file))
		return false;

	if (!FileReadData(&_cost, 4, 1, file))
		return false;

	if (_cost < 0)
		_cost = 0;

	if (!FileReadData(&_maxCpus, 4, 1, file))
		return false;

	if (!FileReadData(&_maxMemory, 4, 1, file))
		return false;

	if (!FileReadData(&_maxUpgrades, 4, 1, file))
		return false;

	if (!FileReadData(&_maxSecurity, 4, 1, file))
		return false;

	if (!FileReadData(&_bandwidth, 4, 1, file))
		return false;

	if (!FileReadData(&_width, 4, 1, file))
		return false;

	if (!FileReadData(&_height, 4, 1, file))
		return false;

	if (!LoadDArrayGatewayDefLocation(&_cpuLocations, file))
		return false;

	if (!LoadDArrayGatewayDefLocation(&_memoryLocations, file))
		return false;

	if (!LoadDArrayGatewayDefLocation(&_securityLocations, file))
		return false;

	if (!FileReadData(&_modemX, 4, 1, file))
		return false;

	if (!FileReadData(&_modemY, 4, 1, file))
		return false;

	if (!FileReadData(&_powerX, 4, 1, file))
		return false;

	if (!FileReadData(&_powerY, 4, 1, file))
		return false;

	return true;
}

void GatewayDef::Save(FILE* file)
{
	SaveDynamicString(_name, 0x80, file);
	SaveDynamicString(_description, 0x100, file);
	SaveDynamicString(_filename, 0x100, file);
	SaveDynamicString(_thumbnail, 0x100, file);
	fwrite(&_cost, 4, 1, file);
	fwrite(&_maxCpus, 4, 1, file);
	fwrite(&_maxMemory, 4, 1, file);
	fwrite(&_maxUpgrades, 4, 1, file);
	fwrite(&_maxSecurity, 4, 1, file);
	fwrite(&_bandwidth, 4, 1, file);
	fwrite(&_width, 4, 1, file);
	fwrite(&_height, 4, 1, file);
	SaveDArrayGatewayDefLocation(&_cpuLocations, file);
	SaveDArrayGatewayDefLocation(&_memoryLocations, file);
	SaveDArrayGatewayDefLocation(&_securityLocations, file);
	fwrite(&_modemX, 4, 1, file);
	fwrite(&_modemY, 4, 1, file);
	fwrite(&_powerX, 4, 1, file);
	fwrite(&_powerY, 4, 1, file);
}

const char* GatewayDef::GetID()
{
	return "GATEWAYDEF";
}

void GatewayDef::LoadGatewayDefinition(std::ifstream& stream)
{
	char temp[0x100];
	stream >> std::ws;
	stream.getline(_name, 0x80, '\r');
	if ((temp[0] = stream.get()) != '\n')
		stream.rdbuf()->sputbackc(temp[0]);

	stream >> std::ws >> _cost;
	stream >> std::ws >> _maxCpus;
	stream >> std::ws >> _maxMemory;
	stream >> std::ws >> _maxUpgrades;
	stream >> std::ws >> _maxSecurity;
	stream >> std::ws >> _bandwidth;
	stream >> std::ws >> _width;
	stream >> std::ws >> _height;

	if (_cost < 0)
		_cost = 0;

	stream >> std::ws;
	stream.getline(_description, 0x100, '\r');
	if ((temp[0] = stream.get()) != '\n')
		stream.rdbuf()->sputbackc(temp[0]);

	// TODO: actually fix this properly
	if (strstr(_description,
			   "An embedded broad-band radio transmitter provides total anonymity for the user - you cannot be caught with this gateway.") != nullptr)
	{
		strncpy(_description, "A good small all rounder.", 0x100);
	}

	if (_maxCpus > 0)
	{
		_cpuLocations.SetSize(_maxCpus);
		stream >> std::ws >> temp;

		for (auto i = 0; i < _maxCpus; i++)
		{
			const auto loc = new GatewayDefLocation();
			stream >> std::ws >> loc->X >> std::ws >> loc->Y;
			_cpuLocations.PutData(loc, i);
		}
	}

	if (_maxMemory > 0)
	{
		_memoryLocations.SetSize(_maxMemory);
		stream >> std::ws >> temp;

		for (auto i = 0; i < _maxMemory; i++)
		{
			const auto loc = new GatewayDefLocation();
			stream >> std::ws >> loc->X >> std::ws >> loc->Y;
			_memoryLocations.PutData(loc, i);
		}
	}

	if (_maxSecurity > 0)
	{
		_securityLocations.SetSize(_maxSecurity);
		stream >> std::ws >> temp;

		for (auto i = 0; i < _maxSecurity; i++)
		{
			const auto loc = new GatewayDefLocation();
			stream >> std::ws >> loc->X >> std::ws >> loc->Y;
			_securityLocations.PutData(loc, i);
		}
	}

	stream >> std::ws >> temp;
	stream >> std::ws >> _modemX >> std::ws >> _modemY;

	stream >> std::ws >> temp;
	stream >> std::ws >> _powerX >> std::ws >> _powerY;
}

void GatewayDef::SetFilename(const char* filename)
{
	UplinkStrncpy(this->_filename, filename, 0x100);
}

void GatewayDef::SetThumbnail(const char* thumbnail)
{
	UplinkStrncpy(this->_thumbnail, thumbnail, 0x100);
}

bool GatewayDef::VerifyCorrectness()
{
	if (_maxCpus > 0 && _maxMemory > 0 && _bandwidth > 0)
		return true;

	printf("WARNING : GatewayDef::VerifyCorrectness, Gateway '%s' loaded from '%s' as invald values, "
		   "(maxcpus <= %d || maxmemory <= %d || bandwidth <= %d )\n",
		   _name, _filename, _maxCpus, _maxMemory, _bandwidth);
	return false;
}

int GatewayDef::GatewayDefComparator(GatewayDef** a, GatewayDef** b)
{
	const auto left = *a;
	const auto right = *b;

	if (left->_cost > right->_cost)
		return 1;

	if (left->_cost < right->_cost)
		return -1;

	return strcmp(left->_name, right->_name);
}
