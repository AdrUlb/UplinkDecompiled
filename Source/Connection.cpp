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
	SaveID_END(file);
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

void Connection::SetOwner(const char* value)
{
	UplinkStrncpy(owner, value, 0x80);
}
