#include <PhoneDialler.hpp>

#include <Eclipse.hpp>
#include <Globals.hpp>
#include <Opengl.hpp>
#include <RedShirt.hpp>
#include <ScriptLibrary.hpp>
#include <Sg.hpp>

PhoneDialler::PhoneDialler()
{
	ip = nullptr;
	info = nullptr;
	nextScene = PhoneDiallerNextScene::Unknown;
	lastUpdateTime = 0;
	ipIndex = -1;
}

PhoneDialler::~PhoneDialler()
{
	if (ip != nullptr)
		delete[] ip;

	if (info != nullptr)
		delete[] info;
}

void PhoneDialler::Remove()
{
	if (IsVisible())
	{
		EclRemoveButton("dialler_bg");
		EclRemoveButton("one");
		EclRemoveButton("two");
		EclRemoveButton("three");
		EclRemoveButton("four");
		EclRemoveButton("five");
		EclRemoveButton("six");
		EclRemoveButton("seven");
		EclRemoveButton("eight");
		EclRemoveButton("nine");
		EclRemoveButton("dialler_number");
	}
	ipIndex = -1;
}

bool PhoneDialler::IsVisible()
{
	return EclGetButton("dialler_number") != nullptr;
}

bool PhoneDialler::UpdateSpecial()
{
	if (EclGetAccurateTime() - lastUpdateTime > 109)
		UpdateDisplay();

	return ipIndex == -1;
}

void PhoneDialler::UpdateDisplay()
{
	if (ipIndex == -1)
	{
		lastUpdateTime = EclGetAccurateTime();
		return;
	}

	const auto dialerNumberButton = EclGetButton("dialler_number");

	if (dialerNumberButton == nullptr)
	{
		ipIndex = -1;
		lastUpdateTime = EclGetAccurateTime();
		return;
	}

	// Dialler animation done, run next scene
	if (ipIndex >= 0 && static_cast<unsigned>(ipIndex) >= strlen(ip))
	{
		switch (nextScene)
		{
			case PhoneDiallerNextScene::Script92:
				ScriptLibrary::RunScript(92);
				break;
			case PhoneDiallerNextScene::Script93:
				ScriptLibrary::RunScript(93);
				break;
			case PhoneDiallerNextScene::WorldMap:
				UplinkAbort("TODO: implement nextScene=PhoneDiallerNextScene::WorldMap");
				break;
			case PhoneDiallerNextScene::Finance:
				UplinkAbort("TODO: implement nextScene=PhoneDiallerNextScene::Finance");
				break;
			case PhoneDiallerNextScene::Links:
				UplinkAbort("TODO: implement nextScene=PhoneDiallerNextScene::Links");
				break;
			default:
			{
				char s[0x40];
				UplinkSnprintf(s, 0x40, "Unrecognised nextScene=%d", nextScene);
				UplinkAbort(s);
			}
		}
		ipIndex = -1;
		return;
	}

	char c;
	do
	{
		c = ip[ipIndex++];
	} while (ipIndex >= 0 && static_cast<unsigned>(ipIndex) < strlen(ip) && (c < '1' || c > '9'));

	if (c >= '1' && c <= '9')
	{
		EclHighlightButton(buttonNames[c - '1']);

		char s[0x40];
		UplinkStrncpy(s, ip, 0x40);

		s[ipIndex] = 0;
		dialerNumberButton->SetCaption(s);

		char soundFileName[0x100];
		UplinkSnprintf(soundFileName, 0x100, "sounds/%d.wav", c - '0');
		SgPlaySound(RsArchiveFileOpen(soundFileName), soundFileName);
	}

	this->lastUpdateTime = EclGetAccurateTime();
}

void PhoneDialler::DialNumber(int x, int y, const char* ip, PhoneDiallerNextScene nextScene, const char* info)
{
	UplinkAssert(ip != nullptr);
	if (this->ip != nullptr)
		delete[] this->ip;

	this->ip = new char[strlen(ip) + 1];
	strcpy(this->ip, ip);

	if (this->info != nullptr)
	{
		delete[] this->info;
		this->info = nullptr;
	}

	if (info != nullptr)
	{
		this->info = new char[strlen(info) + 1];
		strcpy(this->info, info);
	}

	this->nextScene = nextScene;
	lastUpdateTime = 0;
	ipIndex = 0;
	app->RegisterPhoneDialler(this);
	Create(x, y);
}

void PhoneDialler::Create(int x, int y)
{
	if (IsVisible())
		return;

	EclRegisterButton((x - 10), (y - 10), 120, 140, " ", "dialler_bg");
	EclRegisterButtonCallbacks("dialler_bg", BackgroundDraw, 0, 0, nullptr);
	EclRegisterButton(x, y, 29, 29, "1", "one");
	EclRegisterButton((x + 36), y, 29, 29, "2", "two");
	EclRegisterButton((x + 72), y, 29, 29, "3", "three");
	EclRegisterButton(x, (y + 36), 29, 29, "4", "four");
	EclRegisterButton((x + 36), (y + 36), 29, 29, "5", "five");
	EclRegisterButton((x + 72), (y + 36), 29, 29, "6", "six");
	EclRegisterButton(x, (y + 72), 29, 29, "7", "seven");
	EclRegisterButton((x + 36), (y + 72), 29, 29, "8", "eight");
	EclRegisterButton((x + 72), (y + 72), 29, 29, "9", "nine");

	button_assignbitmaps("one", "dialler/one.tif", "dialler/one_h.tif", "dialler/one_h.tif");
	button_assignbitmaps("two", "dialler/two.tif", "dialler/two_h.tif", "dialler/two_h.tif");
	button_assignbitmaps("three", "dialler/three.tif", "dialler/three_h.tif", "dialler/three_h.tif");
	button_assignbitmaps("four", "dialler/four.tif", "dialler/four_h.tif", "dialler/four_h.tif");
	button_assignbitmaps("five", "dialler/five.tif", "dialler/five_h.tif", "dialler/five_h.tif");
	button_assignbitmaps("six", "dialler/six.tif", "dialler/six_h.tif", "dialler/six_h.tif");
	button_assignbitmaps("seven", "dialler/seven.tif", "dialler/seven_h.tif", "dialler/seven_h.tif");
	button_assignbitmaps("eight", "dialler/eight.tif", "dialler/eight_h.tif", "dialler/eight_h.tif");
	button_assignbitmaps("nine", "dialler/nine.tif", "dialler/nine_h.tif", "dialler/nine_h.tif");

	EclRegisterButton(x, (y + 108), 101, 15, " ", "dialler_number");
}
