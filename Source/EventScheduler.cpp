#include <EventScheduler.hpp>

#include <Globals.hpp>

EventScheduler::~EventScheduler()
{
	DeleteLListData(&events);
}

bool EventScheduler::Load(FILE* file)
{
	return LoadLList(reinterpret_cast<LList<UplinkObject*>*>(&events), file);
}

void EventScheduler::Save(FILE* file)
{
	SaveLList(reinterpret_cast<LList<UplinkObject*>*>(&events), file);
	UplinkObject::SaveID_END(file);
}

void EventScheduler::Print()
{
	puts("==== Event Scheduler : ===============================");
	PrintLList(reinterpret_cast<LList<UplinkObject*>*>(&events));
}

void EventScheduler::Update()
{
	for (int i = 0; i != 2; i++)
		if (!UpdateProcessEvents())
			break;
}

const char* EventScheduler::GetID()
{
	return "SCHED";
}

Date* EventScheduler::GetDateOfNextEvent()
{
	const auto event = events.GetData(0);
	if (event == nullptr)
		return nullptr;
	return &event->runDate;
}

bool EventScheduler::UpdateProcessEvents()
{
	const auto eventCount = events.Size();
	LList<UplinkEvent*> runEvents;
	int32_t runEventCount = 0;

	if (events.Size() > 0)
	{
		while (true)
		{
			auto event = events.GetData(0);

			if (event == nullptr)
				break;

			if (!game->GetWorld()->currentDate.After(&event->runDate))
				break;

			runEvents.PutData(event);
			events.RemoveData(0);
			runEventCount++;

			if (!events.ValidIndex(0))
				break;

			event = events.GetData(0);
		}
	}

	while (runEvents.ValidIndex(0))
	{
		const auto event = runEvents.GetData(0);
		if (event == nullptr)
			break;

		event->Run();
		runEvents.RemoveData(0);
		delete event;
	}

	return (eventCount - runEventCount) != events.Size();
}

void EventScheduler::ScheduleEvent(UplinkEvent* event)
{
	UplinkAssert(event != nullptr);

	for (auto i = 0; i < events.Size(); i++)
	{
		const auto thisEvent = events.GetData(i);
		UplinkAssert(thisEvent != nullptr);

		if (event->runDate.Before(&thisEvent->runDate))
		{
			events.PutDataAtIndex(event, i);
			return;
		}
	}

	events.PutDataAtEnd(event);
}
