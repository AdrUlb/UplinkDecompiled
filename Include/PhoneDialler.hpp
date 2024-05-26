#pragma once

#include <LocalInterfaceScreen.hpp>

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
	char* ip;
	char* info;
	PhoneDiallerNextScene nextScene;
	int lastUpdateTime;
	int ipIndex;

	static constexpr char buttonNames[][6] = {"one", "two", "three", "four", "five", "six", "seven", "eight", "nine"};

public:
	PhoneDialler();
	~PhoneDialler() override;
	void Remove() override;
	bool IsVisible() override;
	bool UpdateSpecial();
	void UpdateDisplay();
	void DialNumber(int x, int y, const char* ip, PhoneDiallerNextScene nextScene, const char* info);
	void Create(int x, int y);
};
