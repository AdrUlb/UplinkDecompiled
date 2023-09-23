#pragma once

#include "UplinkObject.hpp"

class Network : UplinkObject
{
	char unknown[0x3C];

public:
	Network();
};
