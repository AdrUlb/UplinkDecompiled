#pragma once

#include <UplinkObject.hpp>

class DialogScreenWidget : UplinkObject
{
	char* name = nullptr;
	char* caption = nullptr;
	char* tooltip = nullptr;
	char* stringData1 = nullptr;
	char* stringData2 = nullptr;
	int x = 0;
	int y = 0;
	int width = 0;
	int height = 0;
	int type = 0;
	int data1 = 0;
	int data2 = 0;

public:
	~DialogScreenWidget() override;
	bool Load(FILE* file) override;
	void Save(FILE* file) override;
	void Print() override;
	const char* GetID() override;
	UplinkObjectId GetOBJECTID() override;
	void SetName(const char* value);
	void SetTYPE(int value);
	void SetPosition(int x, int y);
	void SetSize(int width, int height);
	void SetCaption(const char* value);
	void SetTooltip(const char* value);
	void SetData(int data1, int data2);
	void SetStringData(const char* stringData1, const char* stringData2);
};
