#pragma once

#include <Computer.hpp>
#include <UplinkObject.hpp>

class ComputerScreen : public UplinkObject
{
	char _mainTitle[0x40] = " ";
	char _subTitle[0x40] = " ";
	char _computer[0x40] = " ";

public:
	bool Load(FILE* file) override;
	void Save(FILE* file) override;
	void Print() override;
	const char* GetID() override;
	const char* GetMainTitle();
	const char* GetSubTitle();
	Computer* GetComputer();
	void SetMainTitle(const char* mainTitle);
	void SetSubTitle(const char* subTitle);
	void SetComputer(const char* name);
};
