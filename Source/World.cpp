#include <World.hpp>

void World::ForceNextUpdate()
{
	nextUpdateDate.SetDate(&currentDate);
	nextUpdateDate.AdvanceSecond(-1);
}
