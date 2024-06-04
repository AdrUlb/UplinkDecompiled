#pragma once

#include <ComputerScreens/LinksScreen.hpp>
#include <Interface/RemoteScreens/RemoteInterfaceScreen.hpp>
#include <LList.hpp>

class LinksScreenInterface : public RemoteInterfaceScreen
{
	LList<char*> _displayList;
	LList<char*> _filterList;

public:
	LinksScreenInterface();
	~LinksScreenInterface();
	void Remove() override;
	bool IsVisible() override;
	int ScreenID() override;
	void Create(ComputerScreen* screen) override;
	bool ReturnKeyPressed() override;
	LinksScreen* GetComputerScreen();
	int NumLinksOnScreen();
	void SetFullList();
	void SetFullList(LList<const char*>* links);
	void SetFullList(LList<char*>* links);
	void ApplyFilter(const char* filter);
	void CreateScrollBarAndFilter();
	static void LinkDraw(Button* button, bool highlighted, bool clicked);
	static void AddLinkDraw(Button* button, bool highlighted, bool clicked);
	static void DeleteLinkDraw(Button* button, bool highlighted, bool clicked);
	static void ShowLinkDraw(Button* button, bool highlighted, bool clicked);
	static void FilterDraw(Button* button, bool highlighted, bool clicked);
	static void LinkMouseMove(Button* button);
	static void LinkClick(Button* button);
	static void DeleteLinkClick(Button* button);
	static void ShowLinkClick(Button* button);
	static void AddLinkClick(Button* button);
	static void CloseClick(Button* button);
	static void LinkMouseDown(Button* button);
	static void ShowLinkMouseMove(Button* button);
	static void FilterClick(Button* button);
	static void ScrollChange(const char* scrollboxName, int currentIndex);
};
