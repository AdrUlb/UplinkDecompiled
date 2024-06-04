#pragma once

#include <ComputerScreens/ComputerScreen.hpp>
#include <DialogScreenWidget.hpp>
#include <LList.hpp>

struct DialogScreen : ComputerScreen
{
	LList<DialogScreenWidget*> _widgets;
	char* _returnKeyButtonName = nullptr;
	char* _escapeKeyButtonName = nullptr;

public:
	~DialogScreen() override;
	bool Load(FILE* file) override;
	void Save(FILE* file) override;
	void Print() override;
	const char* GetID() override;
	UplinkObjectId GetOBJECTID() override;
	LList<DialogScreenWidget*>* GetWidgets();
	void AddWidget(const char* name, int type, int x, int y, int width, int height, const char* caption, const char* tooltip);
	void AddWidget(const char* name, int type, int x, int y, int width, int height, const char* caption, const char* tooltip, int data1, int data2,
				   const char* stringData1, const char* stringData2);
	void SetReturnKeyButton(const char* buttonName);
	void SetEscapeKeyButton(const char* buttonName);
};
