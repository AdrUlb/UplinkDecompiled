#include <Date.hpp>
#include <LList.hpp>
#include <UplinkObject.hpp>

class Connection : public UplinkObject
{
	char owner[0x40];
	LList<char*> vlocations;
	bool traceInProgress;
	int traceProgress;
	Date connectionTime;
};
