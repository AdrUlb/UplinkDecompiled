#pragma once

#include <UplinkObject.hpp>
#include <FrameRate.hpp>

class View : UplinkObject
{
	FrameRate frameRate;
public:
	View();
	virtual ~View();
};
