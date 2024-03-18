#pragma once

#include <BTree.hpp>
#include <LList.hpp>
#include <UplinkObject.hpp>
#include <Util.hpp>

struct OptionChange
{
	static constexpr size_t NAME_MAX = 0x40;
	char name[0x40];
	int value;
};

struct ColourOption
{
	float red;
	float green;
	float blue;
};

class Option : UplinkObject
{
	static constexpr size_t NAME_MAX = 0x40;
	static constexpr size_t TOOLTIP_MAX = 0x80;
	char name[NAME_MAX];
	char tooltip[TOOLTIP_MAX];
	bool yesOrNo;
	bool visible;
	int32_t value;

public:
	Option();
	bool Load(FILE* file) override;
	void Save(FILE* file) override;
	void Print() override;
	const char* GetID() override;
	UplinkObjectId GetOBJECTID() override;

	inline void SetName(const char* value)
	{
		UplinkStrncpy(name, value, NAME_MAX);
	}

	inline void SetTooltip(const char* value)
	{
		UplinkStrncpy(tooltip, value, TOOLTIP_MAX);
	}

	inline void SetValue(int value)
	{
		this->value = value;
	}

	inline void SetVisible(bool visible)
	{
		this->visible = visible;
	}

	inline void SetYesOrNo(bool yesOrNo)
	{
		this->yesOrNo = yesOrNo;
	}

	inline const char* GetName()
	{
		return name;
	}

	inline int GetValue()
	{
		return value;
	}

	inline bool GetVisible()
	{
		return visible;
	}

	inline bool GetYesOrNo()
	{
		return yesOrNo;
	}
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

public:
	Options();
	~Options() override;

	bool Load(FILE* file) override;
	void Save(FILE* file) override;
	void Print() override;
	const char* GetID() override;

	void ApplyShutdownChanges();
	void CreateDefaultOptions();
	LList<Option*>* GetAllOptions(const char* search, bool getInvisible);
	ColourOption* GetColour(const char* name);
	Option* GetOption(const char* name);
	int GetOptionValue(const char* name);
	int GetOptionValueOrDefault(const char* name, int defaultValue);
	const char* GetThemeDescription();
	const char* GetThemeName();
	const char* GetThemeTitle();
	bool IsOptionEqualTo(const char* name, int value);
	void RequestShutdownChange(const char* name, int value);
	void SetOptionValue(char const* name, int value);
	void SetOptionValue(const char* name, int value, const char* tooltip, bool yesOrNo, bool visible);
	void SetThemeName(const char* value);
	char* ThemeFilename(const char* name);
};
