#include <PhoneDialler.hpp>

#include <Eclipse.hpp>
#include <Globals.hpp>
#include <Opengl.hpp>

PhoneDialler::PhoneDialler()
{
	ip = nullptr;
	info = nullptr;
	afterDiallerAction = -1;
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
	UplinkAbort("TODO: implement PhoneDialler::UpdateDisplay()");
}

void PhoneDialler::DialNumber(int x, int y, const char* ip, int afterDiallerAction, const char* info)
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

	this->afterDiallerAction = afterDiallerAction;
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
