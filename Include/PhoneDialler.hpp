#pragma once

#include <LocalInterfaceScreen.hpp>

class PhoneDialler : LocalInterfaceScreen
{
	char* ip;
	char* info;
	int afterDiallerAction;
	int lastUpdateTime;
	int ipIndex;

public:
	PhoneDialler();
	~PhoneDialler() override;
	void Remove() override;
	bool IsVisible() override;
	bool UpdateSpecial();
	void UpdateDisplay();
};
