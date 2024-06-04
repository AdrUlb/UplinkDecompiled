#include <EventScheduler.hpp>

#include <Globals.hpp>

EventScheduler::~EventScheduler()
{
	DeleteLListData(reinterpret_cast<LList<UplinkObject*>*>(&_events));
}

bool EventScheduler::Load(FILE* file)
{
	return LoadLList(reinterpret_cast<LList<UplinkObject*>*>(&_events), file);
}

void EventScheduler::Save(FILE* file)
{
	SaveLList(reinterpret_cast<LList<UplinkObject*>*>(&_events), file);
}

void EventScheduler::Print()
{
	puts("==== Event Scheduler : ===============================");
	PrintLList(reinterpret_cast<LList<UplinkObject*>*>(&_events));
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
	const auto event = _events.GetData(0);
	if (event == nullptr)
		return nullptr;
	return &event->GetRunDate();
}

bool EventScheduler::UpdateProcessEvents()
{
	const auto eventCount = _events.Size();
	LList<UplinkEvent*> runEvents;
	auto runEventCount = 0;

	if (_events.Size() > 0)
	{
		while (true)
		{
			auto event = _events.GetData(0);

			if (event == nullptr)
				break;

			if (!game->GetWorld().GetCurrentDate().After(&event->GetRunDate()))
				break;

			runEvents.PutData(event);
			_events.RemoveData(0);
			runEventCount++;

			if (!_events.ValidIndex(0))
				break;

			event = _events.GetData(0);
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

	return (eventCount - runEventCount) != _events.Size();
}

void EventScheduler::ScheduleEvent(UplinkEvent* event)
{
	UplinkAssert(event != nullptr);

	for (auto i = 0; i < _events.Size(); i++)
	{
		const auto thisEvent = _events.GetData(i);
		UplinkAssert(thisEvent != nullptr);

		if (event->GetRunDate().Before(&thisEvent->GetRunDate()))
		{
			_events.PutDataAtIndex(event, i);
			return;
		}
	}

	_events.PutDataAtEnd(event);
}
