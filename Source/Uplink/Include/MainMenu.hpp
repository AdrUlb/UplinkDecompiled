#pragma once

#include <UplinkObject.hpp>
#include <Interface.hpp>

class MainMenu : UplinkObject
{
	int RunningScreenIndex;
	Interface* interface;

public:
	MainMenu();

private:
	virtual ~MainMenu();

	/*virtual bool Load(FILE* file) override;
	virtual void Save(FILE* file) override;
	virtual void Print() override;
	virtual void Update() override;
	virtual const char* GetID() override;*/
};
