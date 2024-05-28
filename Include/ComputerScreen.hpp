#pragma once

#include <UplinkObject.hpp>

class ComputerScreen : public UplinkObject
{
	char mainTitle[0x40] = " ";
	char subTitle[0x40] = " ";
	char computer[0x40] = " ";

public:
	bool Load(FILE* file) override;
	void Save(FILE* file) override;
	void Print() override;
	const char* GetID() override;
	void SetComputer(const char* name);
	void SetMainTitle(const char* value);
	void SetSubTitle(const char* value);
};
