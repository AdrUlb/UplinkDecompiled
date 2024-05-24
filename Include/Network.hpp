#pragma once

#include <DArray.hpp>
#include <UplinkObject.hpp>
#include <ctime>

// TODO
class Network
{
public:
	bool IsActive()
	{
		static auto called = false;
		if (!called)
		{
			puts("TODO: implement Network::IsActive()");
			called = true;
		}

		return false;
	}
};
