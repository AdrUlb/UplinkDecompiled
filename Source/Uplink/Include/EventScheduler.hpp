#pragma once

#include <UplinkObject.hpp>
#include <LList.hpp>
#include <UplinkEvent.hpp>

class EventScheduler : UplinkObject
{
	LList<UplinkEvent*> Events;
};
