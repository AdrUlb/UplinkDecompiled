#pragma once

#include "UplinkObject.hpp"
#include "BTree.hpp"
#include "LList.hpp"

#define OPTION_NAME_MAX 0x40
#define OPTION_TOOLTIP_MAX 0x80

#define OPTIONS_THEMENAME_MAX 0x80
#define OPTIONS_UNKNOWN_MAX 0x80
#define OPTIONS_THEMETITLE_MAX 0x80
#define OPTIONS_THEMEDESCRIPTION_MAX 0x400

class ColorOption
{
	float unknown1;
	float unknown2;
	float unknown3;
};

class Option : UplinkObject
{
	char name[OPTION_NAME_MAX];
	char tooltip[OPTION_TOOLTIP_MAX];
	bool yesNo;
	bool visible;
	int value;

	Option();
public:
	virtual ~Option();
private:
	bool Load(FILE* file) override;
	void Save(FILE* file) override;
	void Print() override;
	void Update() override;
	const char* GetID() override;
	int GetOBJECTID() override;
	void SetName(const char* name);
	void SetTooltip(const char* tooltip);
	void SetValue(int value);
	void SetVisible(bool visible);
	void SetYesOrNo(bool value);
};

class Options : UplinkObject
{
	BTree<Option*> options;
	LList<OptionChange*> changes;
	char themeName[OPTIONS_THEMENAME_MAX];
	char unknown[OPTIONS_UNKNOWN_MAX];
	char themeTitle[OPTIONS_THEMETITLE_MAX];
	char themeDescription[OPTIONS_THEMEDESCRIPTION_MAX];
	BTree<ColorOption*> colorOptions;

public:
	Options();
	virtual ~Options();
	virtual bool Load(FILE* file);
private:
	virtual void Save(FILE* file);
	virtual void Print();
	virtual void Update();
	virtual const char* GetID();
};
