#include <LanComputer.hpp>

struct LanCluster
{
	DArray<LanComputerSystem*> Systems;
	DArray<LanComputerLink*> Links;
	int Input = -1;
	int Output = -1;
	int Field_38 = 0;
	int Field_3c = 0;

	~LanCluster();
};
