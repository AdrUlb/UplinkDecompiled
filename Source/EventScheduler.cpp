#include <EventScheduler.hpp>

Date* EventScheduler::GetDateOfNextEvent()
{
	const auto event = events.GetData(0);
	if (event == nullptr)
		return nullptr;
	return &event->runDate;
}