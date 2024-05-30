#include <NotificationEvent.hpp>

#include <Globals.hpp>
#include <Util.hpp>
#include <cstdio>

bool NotificationEvent::Load(FILE* file)
{
	if (!UplinkEvent::Load(file))
		return false;

	if (!FileReadData(&type, 4, 1, file))
		return false;

	return true;
}

void NotificationEvent::Save(FILE* file)
{
	UplinkEvent::Save(file);
	fwrite(&type, 4, 1, file);
}

void NotificationEvent::Print()
{
	printf("NotificationEvent : TYPE = %d\n", static_cast<int>(type));
	UplinkEvent::Print();
}

const char* NotificationEvent::GetID()
{
	return "EVT_NOT";
}

UplinkObjectId NotificationEvent::GetOBJECTID()
{
	return UplinkObjectId::NotificationEvent;
}

void NotificationEvent::Run()
{
	switch (type)
	{
		case NotificationType::GrowCompanies:
			ApplyMonthlyGrowth();
			return;
		case NotificationType::GenerateNewMissions:
			GenerateNewMissions();
			return;
		case NotificationType::CheckForSecurityBreaches:
			CheckForSecurityBreaches();
			return;
		case NotificationType::CheckMissionDueDates:
			CheckMissionDueDates();
			return;
		case NotificationType::CheckRecentHackCount:
			CheckRecentHackCount();
			return;
		case NotificationType::GiveMissionToNpc:
			GiveMissionToNpc();
			return;
		case NotificationType::PayUplinkMonthlyFee:
			PayUplinkMonthlyFee();
			return;
		case NotificationType::ExpireOldStuff:
			ExpireOldStuff();
			return;
		case NotificationType::AddInterestToLoans:
			AddInterestToLoans();
			return;
		case NotificationType::DemoGameOver:
			DemoGameOver();
			return;
		case NotificationType::DemoGenerateMission:
			DemoGenerateNewMission();
			return;
		case NotificationType::BuyAgentList:
			BuyAgentList();
			return;
		case NotificationType::AgentsOnListDie:
			AgentsOnListDie();
			return;
		case NotificationType::WarezGameOver:
			WarezGameOver();
			return;
		default:
			UplinkAbort("Unrecognised notification type: %d", type);
	}
}

char* NotificationEvent::GetShortString()
{
	const char* str;

	switch (type)
	{
		case NotificationType::Unknown:
			str = "Unknown notification";
			break;
		case NotificationType::GrowCompanies:
			str = "Grow companies";
			break;
		case NotificationType::GenerateNewMissions:
			str = "Generate new mission";
			break;
		case NotificationType::CheckForSecurityBreaches:
			str = "Check for security breaches";
			break;
		case NotificationType::CheckMissionDueDates:
			str = "Check mission due dates";
			break;
		case NotificationType::CheckRecentHackCount:
			str = "Check recent hack count";
			break;
		case NotificationType::GiveMissionToNpc:
			str = "Give mission to NPC";
			break;
		case NotificationType::PayUplinkMonthlyFee:
			str = "Pay Uplink monthly fee";
			break;
		case NotificationType::ExpireOldStuff:
			str = "Expire old stuff";
			break;
		case NotificationType::AddInterestToLoans:
			str = "Add Interest to loans";
			break;
		case NotificationType::DemoGameOver:
			str = "Demo Game Over";
			break;
		case NotificationType::DemoGenerateMission:
			str = "Demo Generate Mission";
			break;
		case NotificationType::BuyAgentList:
			str = "Buy Agent List";
			break;
		case NotificationType::AgentsOnListDie:
			str = "Agents on list die";
			break;
		case NotificationType::WarezGameOver:
			str = "Warez Game Over";
			break;
		default:
			UplinkAbort("Unrecognised notification type: %d", type);
	}

	const auto ret = new char[strlen(str) + 1];
	strcpy(ret, str);

	return ret;
}

char* NotificationEvent::GetLongString()
{
	return GetShortString();
}

void NotificationEvent::SetTYPE(NotificationType type)
{
	this->type = type;
}

void NotificationEvent::ScheduleStartingEvents()
{
	const auto notif1 = new NotificationEvent();
	notif1->SetTYPE(NotificationType::GrowCompanies);
	notif1->SetRunDate(&game->GetWorld()->currentDate);
	game->GetWorld()->eventScheduler.ScheduleEvent(notif1);

	const auto notif2 = new NotificationEvent();
	notif2->SetTYPE(NotificationType::GenerateNewMissions);
	notif2->SetRunDate(&game->GetWorld()->currentDate);
	game->GetWorld()->eventScheduler.ScheduleEvent(notif2);

	const auto notif3 = new NotificationEvent();
	notif3->SetTYPE(NotificationType::CheckForSecurityBreaches);
	notif3->SetRunDate(&game->GetWorld()->currentDate);
	game->GetWorld()->eventScheduler.ScheduleEvent(notif3);

	const auto notif4 = new NotificationEvent();
	notif4->SetTYPE(NotificationType::CheckMissionDueDates);
	notif4->SetRunDate(&game->GetWorld()->currentDate);
	game->GetWorld()->eventScheduler.ScheduleEvent(notif4);

	Date date;

	date.SetDate(&game->GetWorld()->currentDate);
	date.AdvanceMonth(1);
	date.SetDate(1, 1, 1, 1, date.GetMonth(), date.GetYear());
	const auto notif5 = new NotificationEvent();
	notif5->SetTYPE(NotificationType::CheckRecentHackCount);
	notif5->SetRunDate(&date);
	game->GetWorld()->eventScheduler.ScheduleEvent(notif5);

	const auto notif6 = new NotificationEvent();
	notif6->SetTYPE(NotificationType::GiveMissionToNpc);
	notif6->SetRunDate(&game->GetWorld()->currentDate);
	game->GetWorld()->eventScheduler.ScheduleEvent(notif6);

	date.SetDate(&game->GetWorld()->currentDate);
	date.AdvanceMonth(1);
	date.SetDate(1, 1, 1, 1, date.GetMonth(), date.GetYear());
	const auto notif7 = new NotificationEvent();
	notif7->SetTYPE(NotificationType::PayUplinkMonthlyFee);
	notif7->SetRunDate(&date);
	game->GetWorld()->eventScheduler.ScheduleEvent(notif7);

	const auto notif8 = new NotificationEvent();
	notif8->SetTYPE(NotificationType::ExpireOldStuff);
	notif8->SetRunDate(&game->GetWorld()->currentDate);
	game->GetWorld()->eventScheduler.ScheduleEvent(notif8);

	const auto notif9 = new NotificationEvent();
	notif9->SetTYPE(NotificationType::AddInterestToLoans);
	notif9->SetRunDate(&game->GetWorld()->currentDate);
	game->GetWorld()->eventScheduler.ScheduleEvent(notif9);
}

void NotificationEvent::ApplyMonthlyGrowth()
{

	const auto companies = game->GetWorld()->companies.ConvertToDArray();
	for (auto i = 0; i < companies->Size(); i++)
	{
		if (!companies->ValidIndex(i))
			continue;

		companies->GetData(i)->Grow(30);
		companies->GetData(i)->VaryGrowth();
	}

	delete companies;

	Date date;
	date.SetDate(&runDate);
	date.AdvanceMonth(1);
	auto year = date.GetYear();
	auto month = date.GetMonth();
	auto hour = date.GetHour();
	auto minute = date.GetMinute();
	date.SetDate(date.GetSecond(), minute, hour, 1, month, year);

	const auto event = new NotificationEvent();
	event->SetTYPE(NotificationType::GrowCompanies);
	event->SetRunDate(&date);
	game->GetWorld()->eventScheduler.ScheduleEvent(event);
}

void NotificationEvent::GenerateNewMissions()
{
	puts("TODO: implement NotificationEvent::GenerateNewMissions()");
}

void NotificationEvent::CheckForSecurityBreaches()
{
	puts("TODO: implement NotificationEvent::CheckForSecurityBreaches()");
}

void NotificationEvent::CheckMissionDueDates()
{
	puts("TODO: implement NotificationEvent::CheckMissionDueDates()");
}

void NotificationEvent::CheckRecentHackCount()
{
	puts("TODO: implement NotificationEvent::CheckRecentHackCount()");
}

void NotificationEvent::GiveMissionToNpc()
{
	puts("TODO: implement NotificationEvent::GiveMissionToNpc()");
}

void NotificationEvent::PayUplinkMonthlyFee()
{
	puts("TODO: implement NotificationEvent::PayUplinkMonthlyFee()");
}

void NotificationEvent::ExpireOldStuff()
{
	puts("TODO: implement NotificationEvent::ExpireOldStuff()");
}

void NotificationEvent::AddInterestToLoans()
{
	puts("TODO: implement NotificationEvent::AddInterestToLoans()");
}

void NotificationEvent::DemoGameOver()
{
	puts("TODO: implement NotificationEvent::DemoGameOver()");
}

void NotificationEvent::DemoGenerateNewMission()
{
	puts("TODO: implement NotificationEvent::DemoGenerateNewMission()");
}

void NotificationEvent::BuyAgentList()
{
	puts("TODO: implement NotificationEvent::BuyAgentList()");
}

void NotificationEvent::AgentsOnListDie()
{
	puts("TODO: implement NotificationEvent::AgentsOnListDie()");
}

void NotificationEvent::WarezGameOver()
{
	puts("TODO: implement NotificationEvent::WarezGameOver()");
}
