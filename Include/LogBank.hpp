#pragma once

#include <AccessLog.hpp>
#include <DArray.hpp>
#include <UplinkObject.hpp>

class LogBank : UplinkObject
{
	DArray<AccessLog*> accessLogs1;
	DArray<AccessLog*> accessLogs2;
};
