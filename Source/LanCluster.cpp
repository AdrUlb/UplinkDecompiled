#include <LanCluster.hpp>

LanCluster::~LanCluster()
{
	DeleteDArrayDataD(&Links);
}
