#pragma once

#include <Interface/LocalScreens/LocalInterfaceScreen.hpp>

enum class PhoneDiallerNextScene
{
	Unknown = -1,
	Script92 = 1,
	Script93,
	WorldMap,
	Finance,
	Links
};

class PhoneDialler : LocalInterfaceScreen
{
	char* _ip;
	char* _info;
	PhoneDiallerNextScene _nextScene;
	int _lastUpdateTime;
	int _ipIndex;

	static constexpr char _buttonNames[][6] = {"one", "two", "three", "four", "five", "six", "seven", "eight", "nine"};

public:
	using LocalInterfaceScreen::Create;
	PhoneDialler();
	~PhoneDialler() override;
	void Remove() override;
	bool IsVisible() override;
	bool UpdateSpecial();
	void UpdateDisplay();
	void DialNumber(int x, int y, const char* ip, PhoneDiallerNextScene nextScene, const char* info);
	void Create(int x, int y);
};
