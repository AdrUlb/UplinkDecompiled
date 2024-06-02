#include <Connection.hpp>

#include <Globals.hpp>
#include <Util.hpp>

Connection::~Connection()
{
	DeleteLListData(&_vlocations);
}

bool Connection::Load(FILE* file)
{
	if (strcmp(game->GetLoadedSavefileVer(), "SAV59") >= 0)
	{
		if (!LoadDynamicStringBuf(_owner, 0x40, file))
			return false;
	}
	else
	{
		auto success = FileReadData(_owner, 0x40, 1, file);
		this->_owner[0x3F] = 0;
		if (!success)
			return false;
	}

	if (!LoadDynamicStringBuf(_owner, 0x40, file))
		return false;

	if (!FileReadData(&_traceInProgress, 1, 1, file))
		return false;

	if (!FileReadData(&_traceProgress, 4, 1, file))
		return false;

	if (!LoadLList(&this->_vlocations, file))
		return false;

	return true;
}

void Connection::Save(FILE* file)
{
	SaveDynamicString(_owner, 0x40, file);
	fwrite(&_traceInProgress, 1, 1, file);
	fwrite(&_traceProgress, 4, 1, file);
	SaveLList(&_vlocations, file);
}

void Connection::Print()
{
	printf("Connection : Owner = %s, TraceInProgress = %d, TraceProgress = %d\n", _owner, _traceInProgress, _traceProgress);
	PrintLList(&this->_vlocations);
}

const char* Connection::GetID()
{
	return "CONNECT";
}
const char* Connection::GetTarget()
{
	if (_vlocations.ValidIndex(0))
		return _vlocations.GetData(_vlocations.Size() - 1);

	return nullptr;
}

Person* Connection::GetOwner()
{
	const auto ret = game->GetWorld()->GetPerson(_owner);
	UplinkAssert(ret != nullptr);
	return ret;
}

void Connection::SetOwner(const char* owner)
{
	UplinkStrncpy(_owner, owner, 0x80);
}

void Connection::Reset()
{
	DeleteLListData(&_vlocations);
	_vlocations.Empty();
	AddVLocation(GetOwner()->localHost);
	_traceInProgress = false;
	_traceProgress = 0;
}

void Connection::AddVLocation(const char* ip)
{
	if (LocationIncluded(ip) != 0)
		return;

	char* str = new char[0x18];
	UplinkStrncpy(str, ip, 0x18);
	_vlocations.PutDataAtEnd(str);
}

bool Connection::LocationIncluded(const char* ip)
{
	for (auto i = 0; i < _vlocations.Size(); i++)
		if (strcmp(_vlocations.GetData(i), ip) == 0)
			return true;

	return false;
}

void Connection::Connect()
{
	const auto target = GetTarget();

	GetOwner()->SetRemoteHost(target);
	_traceProgress = 0;
	_traceInProgress = false;
	_connectionTime.SetDate(&game->GetWorld()->currentDate);

	const auto vlocation = game->GetWorld()->GetVLocation(target);
	UplinkAssert(vlocation != nullptr);

	const auto computer = vlocation->GetComputer();
	UplinkAssert(computer != nullptr);

	const auto rax_5 = computer->GetSecurity().IsAnythingDisabled();
	if (rax_5)
		BeginTrace();

	if (strcmp("PLAYER", _owner) == 0) // cond3
	{
		// SecurityMonitor::BeginAttack(); and so on
		puts("TODO: Connection::Connect()");
	}

	for (auto j = 0; j < _vlocations.Size(); j++)
	{
		const auto vloc = game->GetWorld()->GetVLocation(_vlocations.GetData(j));
		UplinkAssert(vloc != nullptr);

		const auto comp = vloc->GetComputer();
		if (comp == 0)
			continue;

		static auto called = false;
		if (!called)
		{
			puts("TODO: Connection::Connect()");
			called = true;
		}

		const auto accessLog = new AccessLog();

		const auto lastLoc = _vlocations.Size() - 1;

		if (j == lastLoc)
		{
			accessLog->SetProperties(game->GetWorld()->currentDate, _vlocations.GetData(lastLoc - 1), _owner, 0, 2);
			comp->GetLogBank().AddLog(accessLog, -1);
			continue;
		}

		if (j == 0)
			accessLog->SetProperties(game->GetWorld()->currentDate, "LOCAL", _owner, 0, 4);
		else
			accessLog->SetProperties(game->GetWorld()->currentDate, _vlocations.GetData(j - 1), _owner, 0, 5);

		accessLog->SetData1(_vlocations.GetData(j + 1));

		comp->GetLogBank().AddLog(accessLog, -1);
	}
}

void Connection::BeginTrace()
{
	if (this->_traceInProgress)
		return;

	struct VLocation* vlocation = game->GetWorld()->GetVLocation(GetTarget());
	UplinkAssert(vlocation != nullptr);

	struct Computer* computer = vlocation->GetComputer();
	UplinkAssert(computer != nullptr);

	if (computer->GetTraceAction() > 0)
	{
		_traceInProgress = true;
		_traceProgress = 0;
	}
}
