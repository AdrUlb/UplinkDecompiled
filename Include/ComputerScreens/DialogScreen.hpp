#pragma once

#include <ComputerScreens/ComputerScreen.hpp>
#include <DialogScreenWidget.hpp>
#include <LList.hpp>

struct DialogScreen : ComputerScreen
{
	LList<DialogScreenWidget*> widgets;
	char* returnKeyButton = nullptr;
	char* escapeKeyButton = nullptr;

public:
	~DialogScreen() override;
	bool Load(FILE* file) override;
	void Save(FILE* file) override;
	void Print() override;
	const char* GetID() override;
	UplinkObjectId GetOBJECTID() override;
	void AddWidget(const char* name, int type, int x, int y, int width, int height, const char* caption, const char* tooltip);
	void AddWidget(const char* name, int type, int x, int y, int width, int height, const char* caption, const char* tooltip, int data1, int data2,
				   const char* stringData1, const char* stringData2);
};
