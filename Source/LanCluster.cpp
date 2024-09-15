#include <LanCluster.hpp>

LanCluster::~LanCluster()
{
	DeleteDArrayDataD(reinterpret_cast<DArray<UplinkObject*>*>(&Links));
}
