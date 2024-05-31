#include <Connection.hpp>

#include <Globals.hpp>
#include <Util.hpp>

Connection::~Connection()
{
	DeleteLListData(&vlocations);
}

bool Connection::Load(FILE* file)
{
	if (strcmp(game->GetLoadedSavefileVer(), "SAV59") >= 0)
	{
		if (!LoadDynamicStringBuf(owner, 0x40, file))
			return false;
	}
	else
	{
		auto success = FileReadData(owner, 0x40, 1, file);
		this->owner[0x3F] = 0;
		if (!success)
			return false;
	}

	if (!LoadDynamicStringBuf(owner, 0x40, file))
		return false;

	if (!FileReadData(&traceInProgress, 1, 1, file))
		return false;

	if (!FileReadData(&traceProgress, 4, 1, file))
		return false;

	if (!LoadLList(&this->vlocations, file))
		return false;

	return true;
}

void Connection::Save(FILE* file)
{
	SaveDynamicString(owner, 0x40, file);
	fwrite(&traceInProgress, 1, 1, file);
	fwrite(&traceProgress, 4, 1, file);
	SaveLList(&vlocations, file);
}

void Connection::Print()
{
	printf("Connection : Owner = %s, TraceInProgress = %d, TraceProgress = %d\n", owner, traceInProgress, traceProgress);
	PrintLList(&this->vlocations);
}

const char* Connection::GetID()
{
	return "CONNECT";
}
const char* Connection::GetTarget()
{
	if (vlocations.ValidIndex(0))
		return vlocations.GetData(vlocations.Size() - 1);

	return nullptr;
}

Person* Connection::GetOwner()
{
	const auto ret = game->GetWorld()->GetPerson(owner);
	UplinkAssert(ret != nullptr);
	return ret;
}

void Connection::SetOwner(const char* value)
{
	UplinkStrncpy(owner, value, 0x80);
}

void Connection::Reset()
{
	DeleteLListData(&vlocations);
	vlocations.Empty();
	AddVLocation(GetOwner()->localHost);
	traceInProgress = false;
	traceProgress = 0;
}

void Connection::AddVLocation(const char* ip)
{
	if (LocationIncluded(ip) != 0)
		return;

	char* str = new char[0x18];
	UplinkStrncpy(str, ip, 0x18);
	vlocations.PutDataAtEnd(str);
}

bool Connection::LocationIncluded(const char* ip)
{
	for (auto i = 0; i < vlocations.Size(); i++)
		if (strcmp(vlocations.GetData(i), ip) == 0)
			return true;

	return false;
}

void Connection::Connect()
{
	const auto target = GetTarget();

	GetOwner()->SetRemoteHost(target);
	traceProgress = 0;
	traceInProgress = false;
	connectionTime.SetDate(&game->GetWorld()->currentDate);

	const auto vlocation = game->GetWorld()->GetVLocation(target);
	UplinkAssert(vlocation != nullptr);

	const auto computer = vlocation->GetComputer();
	UplinkAssert(computer != nullptr);

	const auto rax_5 = computer->security.IsAnythingDisabled();
	if (rax_5)
		BeginTrace();

	if (strcmp("PLAYER", owner) == 0) // cond3
	{
		// SecurityMonitor::BeginAttack(); and so on
		puts("TODO: Connection::Connect()");
	}

	for (auto j = 0; j < vlocations.Size(); j++)
	{
		const auto vloc = game->GetWorld()->GetVLocation(vlocations.GetData(j));
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

		const auto lastLoc = vlocations.Size() - 1;

		if (j == lastLoc)
		{
			accessLog->SetProperties(game->GetWorld()->currentDate, vlocations.GetData(lastLoc - 1), owner, 0, 2);
			comp->logBank.AddLog(accessLog, -1);
			continue;
		}

		if (j == 0)
			accessLog->SetProperties(game->GetWorld()->currentDate, "LOCAL", owner, 0, 4);
		else
			accessLog->SetProperties(game->GetWorld()->currentDate, vlocations.GetData(j - 1), owner, 0, 5);

		accessLog->SetData1(vlocations.GetData(j + 1));

		comp->logBank.AddLog(accessLog, -1);
	}
}

void Connection::BeginTrace()
{
	if (this->traceInProgress)
		return;

	struct VLocation* vlocation = game->GetWorld()->GetVLocation(GetTarget());
	UplinkAssert(vlocation != nullptr);

	struct Computer* computer = vlocation->GetComputer();
	UplinkAssert(computer != nullptr);

	if (computer->traceAction > 0)
	{
		traceInProgress = true;
		traceProgress = 0;
	}
}
