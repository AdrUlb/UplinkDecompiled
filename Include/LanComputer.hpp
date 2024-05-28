#pragma once

#include <Computer.hpp>
#include <DArray.hpp>

typedef void LanComputerSystem;
typedef void LanComputerLink;

class LanComputer : public Computer
{
	DArray<LanComputerSystem*> systems;
	DArray<LanComputerLink*> links;
};
