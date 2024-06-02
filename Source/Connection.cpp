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

int Connection::GetSize()
{
	return _vlocations.Size();
}

void Connection::SetOwner(const char* owner)
{
	UplinkStrncpy(_owner, owner, 0x80);
}

const char* Connection::GetGhost()
{
	if (_vlocations.Size() < 2)
		return nullptr;

	return _vlocations.GetData(_vlocations.Size() - 2);
}

void Connection::Reset()
{
	DeleteLListData(&_vlocations);
	_vlocations.Empty();
	AddVLocation(GetOwner()->GetLocalHostIp());
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
	_connectionTime.SetDate(&game->GetWorld()->GetCurrentDate());

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

		const auto connectLog = new AccessLog();

		const auto lastLoc = _vlocations.Size() - 1;

		if (j == lastLoc)
		{
			connectLog->SetProperties(game->GetWorld()->GetCurrentDate(), _vlocations.GetData(lastLoc - 1), _owner, 0, 2);
			comp->GetLogBank().AddLog(connectLog, -1);
			continue;
		}

		if (j == 0)
			connectLog->SetProperties(game->GetWorld()->GetCurrentDate(), "LOCAL", _owner, 0, 4);
		else
			connectLog->SetProperties(game->GetWorld()->GetCurrentDate(), _vlocations.GetData(j - 1), _owner, 0, 5);

		connectLog->SetData1(_vlocations.GetData(j + 1));

		comp->GetLogBank().AddLog(connectLog, -1);
	}
}

void Connection::Disconnect()
{
	const auto targetVlocation = game->GetWorld()->GetVLocation(GetTarget());
	UplinkAssert(targetVlocation != nullptr);

	const auto targetComputer = targetVlocation->GetComputer();
	UplinkAssert(targetComputer != nullptr);

	bool rbp_1 = TraceInProgress() && !Traced();

	const auto disconnectLog = new AccessLog();
	disconnectLog->SetProperties(game->GetWorld()->GetCurrentDate(), _vlocations.GetData((_vlocations.Size() - 2)), _owner, 0, 3);

	if (rbp_1 != 0)
	{
		auto setsus = true;

		for (auto i = targetComputer->GetLogBank().GetAccessLogs().Size(); i >= 0; i--)
		{
			if (!targetComputer->GetLogBank().GetAccessLogs().ValidIndex(i))
				continue;

			const auto log = targetComputer->GetLogBank().GetAccessLogs().GetData(i);
			UplinkAssert(log != nullptr);

			Date date;
			date.SetDate(&log->GetDate());
			date.AdvanceMinute(5);

			if (date.Before(&disconnectLog->GetDate()))
				break;

			if (log->GetTYPE() == 2 && strcmp(log->GetFromName(), _owner) == 0 && strcmp(log->GetFromIp(), disconnectLog->GetFromIp()) == 0)
			{
				log->SetSuspicious(1);
				if (targetComputer->GetLogBank().GetAccessLogsModified().ValidIndex(i) != 0)
					targetComputer->GetLogBank().GetAccessLogsModified().GetData(i)->SetSuspicious(1);
				setsus = false;
				break;
			}
		}

		if (setsus)
			disconnectLog->SetSuspicious(1);
	}

	targetComputer->GetLogBank().AddLog(disconnectLog, -1);

	if (strcmp(_owner, "PLAYER") == 0)
	{
		// SecurityMonitor::EndAttack(); and so on
		puts("TODO: Connection::Disconnect()");
	}
	const auto owner = GetOwner();
	owner->SetRemoteHost(owner->GetLocalHostIp());
	_traceProgress = 0;
	_traceInProgress = false;
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

bool Connection::TraceInProgress()
{
	return _traceInProgress;
}

bool Connection::Traced()
{
	if (!_traceInProgress)
		return 0;

	return _traceProgress == GetSize() - 1;
}
