#pragma once

#include <UplinkObject.hpp>
#include <BTree.hpp>
#include <LList.hpp>

#define OPTION_NAME_MAX 0x40
#define OPTION_TOOLTIP_MAX 0x80

#define OPTIONS_THEMENAME_MAX 0x80
#define OPTIONS_UNKNOWN_MAX 0x80
#define OPTIONS_THEMETITLE_MAX 0x80
#define OPTIONS_THEMEDESCRIPTION_MAX 0x400

class Option : UplinkObject
{
	char name[OPTION_NAME_MAX];
	char tooltip[OPTION_TOOLTIP_MAX];
	bool yesNo;
	bool visible;
public:
	int Value;

	Option();
	virtual ~Option();
private:
	bool Load(FILE* file) override;
	void Save(FILE* file) override;
	void Print() override;
	void Update() override;
	const char* GetID() override;
	int GetOBJECTID() override;
public:
	void SetName(const char* name);
	void SetTooltip(const char* tooltip);
	void SetValue(int value);
	void SetVisible(bool visible);
	void SetYesOrNo(bool value);
};

class ColorOption
{
	float unknown1;
	float unknown2;
	float unknown3;
};

typedef void OptionChange;

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
	bool Load(FILE* file) override;
	void Save(FILE* file) override;
private:
	void Print() override;
	void Update() override;
	const char* GetID() override;
public:
	void CreateDefaultOptions();
	Option* GetOption(const char* name);
	int GetOptionValue(const char* name);
	bool IsOptionEqualTo(const char* name, int value);
	void SetOptionValue(const char* name, int value);
	void SetOptionValue(const char* name, int value, const char* tooltip, bool yesNo, bool visible);
	const char* GetThemeName();
	void SetThemeName(const char* themeName);
};
