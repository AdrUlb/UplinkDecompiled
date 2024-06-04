#include <Computer.hpp>

#include <Game.hpp>
#include <Globals.hpp>

Computer::~Computer()
{
	DeleteDArrayDataD(&_screens);
}

bool Computer::Load(FILE* file)
{
	if (!FileReadData(&_type, 4, 1, file))
		return false;

	if (!LoadDynamicStringBuf(_name, 0x40, file))
		return false;

	if (!LoadDynamicStringBuf(_companyName, 0x40, file))
		return false;

	if (!LoadDynamicStringBuf(_ip, 0x18, file))
		return false;

	if (!FileReadData(&_traceSpeed, 4, 1, file))
		return false;

	if (!FileReadData(&_traceAction, 4, 1, file))
		return false;

	if (!FileReadData(&_recentHacks, 4, 1, file))
		return false;

	if (!FileReadData(&_recentHacksThisMonth, 4, 1, file))
		return false;

	if (!FileReadData(&_recentHacksLastMonth, 4, 1, file))
		return false;

	if (!FileReadData(&_isTargetable, 1, 1, file))
		return false;

	if (!FileReadData(&_isExternallyOpen, 1, 1, file))
		return false;

	if (!FileReadData(&_running, 1, 1, file))
		return false;

	if (!FileReadData(&_revelationInfectedVersion, 4, 1, file))
		return false;

	if (LoadDArray(reinterpret_cast<DArray<UplinkObject*>*>(&_screens), file) == 0)
		return false;

	if (_dataBank.Load(file) == 0)
		return false;

	if (_logBank.Load(file) == 0)
		return false;

	if (_recordBank.Load(file) == 0)
		return false;

	if (_security.Load(file) == 0)
		return false;

	if (_revelationInfectedDate.Load(file) == 0)
		return false;

	return true;
}

void Computer::Save(FILE* file)
{
	fwrite(&_type, 4, 1, file);
	SaveDynamicString(_name, 0x40, file);
	SaveDynamicString(_companyName, 0x40, file);
	SaveDynamicString(_ip, 0x18, file);
	fwrite(&_traceSpeed, 4, 1, file);
	fwrite(&_traceAction, 4, 1, file);
	fwrite(&_recentHacks, 4, 1, file);
	fwrite(&_recentHacksThisMonth, 4, 1, file);
	fwrite(&_recentHacksLastMonth, 4, 1, file);
	fwrite(&_isTargetable, 1, 1, file);
	fwrite(&_isExternallyOpen, 1, 1, file);
	fwrite(&_running, 1, 1, file);
	fwrite(&_revelationInfectedVersion, 4, 1, file);
	SaveDArray(reinterpret_cast<DArray<UplinkObject*>*>(&_screens), file);
	_dataBank.Save(file);
	_logBank.Save(file);
	_recordBank.Save(file);
	_security.Save(file);
	_revelationInfectedDate.Save(file);
}

void Computer::Print()
{
	printf("Computer : Name = %s\n", _name);
	printf("TYPE = %d, Companyname = %s, ip = %s, tracespeed = %d, traceaction = %d, targetable = %d, externally open = %d, running = %d\n", _type,
		   _companyName, _ip, _traceSpeed, _traceAction, _isTargetable, _isExternallyOpen, _running);
	printf("numrecenthacks = %d, numhacksthismonth = %d, numhackslastmonth = %d\n", _recentHacks, _recentHacksThisMonth, _recentHacksLastMonth);
	printf("infected with revelation = %f\n", _revelationInfectedVersion);
	_revelationInfectedDate.Print();
	PrintDArray(reinterpret_cast<DArray<UplinkObject*>*>(&_screens));
	_dataBank.Print();
	_logBank.Print();
	_recordBank.Print();
	_security.Print();
}

void Computer::Update()
{
	if (!_running)
		return;

	static auto called = false;
	if (!called)
	{
		puts("TODO: implement Computer::Update()");
		called = true;
	}
}

const char* Computer::GetID()
{
	return "COMPUT";
}

UplinkObjectId Computer::GetOBJECTID()
{
	return UplinkObjectId::Computer;
}

void Computer::SetName(const char* name)
{
	UplinkStrncpy(_name, name, 0x40);
}

void Computer::SetCompanyName(const char* companyName)
{
	if (companyName == nullptr)
		return;

	UplinkAssert(game->GetWorld().GetCompany(companyName) != nullptr);
	UplinkStrncpy(_companyName, companyName, 0x40);
}

void Computer::SetIP(const char* ip)
{
	UplinkAssert(game->GetWorld().GetVLocation(ip) != nullptr);
	UplinkStrncpy(_ip, ip, 0x18);
}

void Computer::SetIsTargetable(bool isTargetable)
{
	_isTargetable = isTargetable;
}

void Computer::AddComputerScreen(ComputerScreen* screen, int index)
{
	UplinkAssert(screen != nullptr);
	screen->SetComputer(_name);
	if (index == -1)
	{
		_screens.PutData(screen);
		return;
	}

	if (index >= _screens.Size())
		_screens.SetSize(index + 1);

	_screens.PutData(screen, index);
}

int Computer::GetType()
{
	return _type;
}

const char* Computer::GetName()
{
	return _name;
}

const char* Computer::GetCompanyName()
{
	return _companyName;
}

const char* Computer::GetIp()
{
	return _ip;
}

int Computer::GetTraceAction()
{
	return _traceAction;
}

bool Computer::GetIsExternallyOpen()
{
	return _isExternallyOpen;
}

bool Computer::GetRunning()
{
	return _running;
}

DataBank& Computer::GetDataBank()
{
	return _dataBank;
}

LogBank& Computer::GetLogBank()
{
	return _logBank;
}

RecordBank& Computer::GetRecordBank()
{
	return _recordBank;
}

Security& Computer::GetSecurity()
{
	return _security;
}

void Computer::SetTYPE(int type)
{
	_type = type;
}

void Computer::SetTraceSpeed(int traceSpeed)
{
	_traceSpeed = traceSpeed;
}

void Computer::SetIsExternallyOpen(bool isExternallyOpen)
{
	_isExternallyOpen = isExternallyOpen;
}

void Computer::SetTraceAction(int traceAction)
{
	_traceAction = traceAction;
}

ComputerScreen* Computer::GetComputerScreen(int index)
{
	if (!_screens.ValidIndex(index))
		return nullptr;

	return _screens.GetData(index);
}

bool Computer::ChangeSecurityCodes()
{
	puts("TODO: implement Computer::ChangeSecurityCodes()");
	return false;
}

void Computer::GenerateAccessCode(const char* name, const char* code, char* buf, size_t max)
{
	if (max == 0)
		return;

	UplinkSnprintf(buf, max, "%s:'%s', %s:'%s'", "NAME", name, "CODE", code);
}