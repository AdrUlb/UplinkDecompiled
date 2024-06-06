#pragma once

#include <UplinkObject.hpp>

class DialogScreenWidget : public UplinkObject
{
	char* _name = nullptr;
	char* _caption = nullptr;
	char* _tooltip = nullptr;
	char* _stringData1 = nullptr;
	char* _stringData2 = nullptr;
	int _x = 0;
	int _y = 0;
	int _width = 0;
	int _height = 0;
	int _type = 0;
	int _data1 = 0;
	int _data2 = 0;

public:
	~DialogScreenWidget() override;
	bool Load(FILE* file) override;
	void Save(FILE* file) override;
	void Print() override;
	const char* GetID() override;
	UplinkObjectId GetOBJECTID() override;
	const char* GetName();
	const char* GetCaption();
	const char* GetTooltip();
	int GetX();
	int GetY();
	int GetWidth();
	int GetHeight();
	int GetType();
	int GetData1();
	int GetData2();
	void SetName(const char* value);
	void SetCaption(const char* value);
	void SetTooltip(const char* value);
	void SetStringData(const char* stringData1, const char* stringData2);
	void SetPosition(int x, int y);
	void SetSize(int width, int height);
	void SetTYPE(int value);
	void SetData(int data1, int data2);
};
