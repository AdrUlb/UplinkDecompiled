#include <Computer.hpp>

#include <Game.hpp>
#include <Globals.hpp>

Computer::~Computer()
{
	DeleteDArrayDataD(&screens);
}

bool Computer::Load(FILE* file)
{
	if (!FileReadData(&type, 4, 1, file))
		return false;

	if (!LoadDynamicStringBuf(name, 0x40, file))
		return false;

	if (!LoadDynamicStringBuf(companyName, 0x40, file))
		return false;

	if (!LoadDynamicStringBuf(ip, 0x18, file))
		return false;

	if (!FileReadData(&traceSpeed, 4, 1, file))
		return false;

	if (!FileReadData(&traceAction, 4, 1, file))
		return false;

	if (!FileReadData(&recentHacks, 4, 1, file))
		return false;

	if (!FileReadData(&recentHacksThisMonth, 4, 1, file))
		return false;

	if (!FileReadData(&recentHacksLastMonth, 4, 1, file))
		return false;

	if (!FileReadData(&targetable, 1, 1, file))
		return false;

	if (!FileReadData(&externallyOpen, 1, 1, file))
		return false;

	if (!FileReadData(&running, 1, 1, file))
		return false;

	if (!FileReadData(&revelationInfectedVersion, 4, 1, file))
		return false;

	if (LoadDArray(reinterpret_cast<DArray<UplinkObject*>*>(&screens), file) == 0)
		return false;

	if (dataBank.Load(file) == 0)
		return false;

	if (logBank.Load(file) == 0)
		return false;

	if (recordBank.Load(file) == 0)
		return false;

	if (security.Load(file) == 0)
		return false;

	if (revelationInfectedDate.Load(file) == 0)
		return false;

	return true;
}

void Computer::Save(FILE* file)
{
	fwrite(&type, 4, 1, file);
	SaveDynamicString(name, 0x40, file);
	SaveDynamicString(companyName, 0x40, file);
	SaveDynamicString(ip, 0x18, file);
	fwrite(&traceSpeed, 4, 1, file);
	fwrite(&traceAction, 4, 1, file);
	fwrite(&recentHacks, 4, 1, file);
	fwrite(&recentHacksThisMonth, 4, 1, file);
	fwrite(&recentHacksLastMonth, 4, 1, file);
	fwrite(&targetable, 1, 1, file);
	fwrite(&externallyOpen, 1, 1, file);
	fwrite(&running, 1, 1, file);
	fwrite(&revelationInfectedVersion, 4, 1, file);
	SaveDArray(reinterpret_cast<DArray<UplinkObject*>*>(&screens), file);
	dataBank.Save(file);
	logBank.Save(file);
	recordBank.Save(file);
	security.Save(file);
	revelationInfectedDate.Save(file);
	SaveID_END(file);
}

void Computer::Print()
{
	printf("Computer : Name = %s\n", name);
	printf("TYPE = %d, Companyname = %s, ip = %s, tracespeed = %d, traceaction = %d, targetable = %d, externally open = %d, running = %d\n", type,
		   companyName, ip, traceSpeed, traceAction, targetable, externallyOpen, running);
	printf("numrecenthacks = %d, numhacksthismonth = %d, numhackslastmonth = %d\n", recentHacks, recentHacksThisMonth, recentHacksLastMonth);
	printf("infected with revelation = %f\n", revelationInfectedVersion);
	revelationInfectedDate.Print();
	PrintDArray(reinterpret_cast<DArray<UplinkObject*>*>(&screens));
	dataBank.Print();
	logBank.Print();
	recordBank.Print();
	security.Print();
}

void Computer::Update()
{
	puts("TODO: implement Computer::Update()");
}

const char* Computer::GetID()
{
	return "COMPUT";
}

UplinkObjectId Computer::GetOBJECTID()
{
	return UplinkObjectId::Computer;
}

void Computer::SetName(const char* value)
{
	UplinkStrncpy(name, value, 0x40);
}

void Computer::SetCompanyName(const char* value)
{
	if (value == nullptr)
		return;

	UplinkAssert(game->GetWorld()->GetCompany(value) != nullptr);
	UplinkStrncpy(companyName, value, 0x40);
}

void Computer::SetIP(const char* value)
{
	UplinkAssert(game->GetWorld()->GetVLocation(value) != nullptr);
	UplinkStrncpy(ip, value, 0x18);
}

void Computer::SetIsTargetable(bool value)
{
	targetable = value;
}

void Computer::AddComputerScreen(ComputerScreen* screen, int index)
{
	UplinkAssert(screen != nullptr);
	screen->SetComputer(name);
	if (index == -1)
	{
		screens.PutData(screen);
		return;
	}

	if (index >= screens.Size())
		screens.SetSize(index + 1);

	screens.PutData(screen, index);
}

void Computer::SetTYPE(int value)
{
	type = value;
}

void Computer::SetTraceSpeed(int value)
{
	traceSpeed = value;
}
