#pragma once

#include <BTree.hpp>
#include <LList.hpp>
#include <UplinkObject.hpp>
#include <Util.hpp>

static constexpr size_t OPTIONCHANGE_NAME_MAX = 0x40;

static constexpr size_t OPTION_NAME_MAX = 0x40;
static constexpr size_t OPTION_TOOLTIP_MAX = 0x80;

static constexpr size_t OPTIONS_THEMENAME_MAX = 0x80;
static constexpr size_t OPTIONS_THEMEAUTHOR_MAX = 0x80;
static constexpr size_t OPTIONS_THEMETITLE_MAX = 0x80;
static constexpr size_t OPTIONS_THEMEDESCRIPTION_MAX = 0x400;

struct OptionChange
{
	char Name[0x40];
	int Value;
};

struct ColourOption
{
	float Red;
	float Green;
	float Blue;
};

class Option : public UplinkObject
{
	char _name[OPTION_NAME_MAX];
	char _tooltip[OPTION_TOOLTIP_MAX];
	bool _yesOrNo;
	bool _visible;
	int _value;

public:
	Option();
	bool Load(FILE* file) override;
	void Save(FILE* file) override;
	void Print() override;
	const char* GetID() override;
	UplinkObjectId GetOBJECTID() override;

	inline const char* GetName()
	{
		return _name;
	}

	inline int GetValue()
	{
		return _value;
	}

	inline bool GetVisible()
	{
		return _visible;
	}

	inline bool GetYesOrNo()
	{
		return _yesOrNo;
	}

	inline void SetName(const char* name)
	{
		UplinkStrncpy(_name, name, OPTION_NAME_MAX);
	}

	inline void SetTooltip(const char* tooltip)
	{
		UplinkStrncpy(_tooltip, tooltip, OPTION_TOOLTIP_MAX);
	}

	inline void SetValue(int value)
	{
		this->_value = value;
	}

	inline void SetVisible(bool visible)
	{
		this->_visible = visible;
	}

	inline void SetYesOrNo(bool yesOrNo)
	{
		this->_yesOrNo = yesOrNo;
	}
};

class Options : UplinkObject
{

	BTree<Option*> _options;
	LList<OptionChange*> _optionChanges;
	char _themeName[OPTIONS_THEMENAME_MAX];
	char _themeAuthor[OPTIONS_THEMEAUTHOR_MAX];
	char _themeTitle[OPTIONS_THEMETITLE_MAX];
	char _themeDescription[OPTIONS_THEMEDESCRIPTION_MAX];
	BTree<ColourOption*> _colourOptions;

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
