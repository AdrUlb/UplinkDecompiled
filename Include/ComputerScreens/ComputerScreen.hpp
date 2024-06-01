#pragma once

#include <Computer.hpp>
#include <UplinkObject.hpp>

class ComputerScreen : public UplinkObject
{
public:
	char mainTitle[0x40] = " ";
	char subTitle[0x40] = " ";
	char computer[0x40] = " ";

	bool Load(FILE* file) override;
	void Save(FILE* file) override;
	void Print() override;
	const char* GetID() override;
	Computer* GetComputer();
	void SetComputer(const char* name);
	void SetMainTitle(const char* value);
	void SetSubTitle(const char* value);
};
