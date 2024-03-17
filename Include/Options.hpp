#pragma once

#include <BTree.hpp>
#include <LList.hpp>
#include <UplinkObject.hpp>

typedef void ColourOption;
typedef void OptionChange;

class Option : UplinkObject
{
	static constexpr size_t NAME_MAX = 0x40;
	static constexpr size_t TOOLTIP_MAX = 0x80;
	char name[NAME_MAX];
	char tooltip[TOOLTIP_MAX];
	bool yesOrNo;
	bool visible;
	int32_t value;

	Option();
	bool Load(FILE* file) override;
	void Save(FILE* file) override;
	void Print() override;
	const char* GetID() override;
	UplinkObjectId GetOBJECTID() override;

	void SetName(const char* value);
	void SetTooltip(const char* value);
	void SetValue(int value);
	void SetVisible(bool visible);
	void SetYesOrNo(bool yesOrNo);
};

class Options : UplinkObject
{
	static constexpr size_t THEMENAME_MAX = 0x80;
	static constexpr size_t THEMEAUTHOR_MAX = 0x80;
	static constexpr size_t THEMETITLE_MAX = 0x80;
	static constexpr size_t THEMEDESCRIPTION_MAX = 0x400;

	struct BTree<Option*> options;
	struct LList<OptionChange*> optionChanges;
	char themeName[THEMENAME_MAX];
	char themeAuthor[THEMEAUTHOR_MAX];
	char themeTitle[THEMETITLE_MAX];
	char themeDescription[THEMEDESCRIPTION_MAX];
	struct BTree<ColourOption*> colourOptions;

	Options();
	~Options() override;

	bool Load(FILE* file) override;
	void Save(FILE* file) override;
	void Print() override;
	const char* GetID() override;

	void ApplyShutdownChanges();
	void CreateDefaultOptions();
};
