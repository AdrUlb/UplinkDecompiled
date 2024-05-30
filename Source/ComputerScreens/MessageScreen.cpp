#include <ComputerScreens/MessageScreen.hpp>

#include <Util.hpp>

MessageScreen::~MessageScreen()
{
	if (textMessage != nullptr)
		delete[] textMessage;

	if (buttonMessage != nullptr)
		delete[] buttonMessage;
}

bool MessageScreen::Load(FILE* file)
{
	if (!ComputerScreen::Load(file))
		return false;

	if (!FileReadData(&nextPage, 4, 1, file))
		return false;

	if (!LoadDynamicString(textMessage, file))
		return false;

	if (!LoadDynamicString(buttonMessage, file))
		return false;

	if (!FileReadData(&mailThisToMe, 1, 1, file))
		return false;

	return true;
}

void MessageScreen::Save(FILE* file)
{
	ComputerScreen::Save(file);
	fwrite(&nextPage, 4, 1, file);
	SaveDynamicString(textMessage, file);
	SaveDynamicString(buttonMessage, file);
	fwrite(&mailThisToMe, 1, 1, file);
}

void MessageScreen::Print()
{
	puts("MessageScreen : ");
	ComputerScreen::Print();
	printf("NextPage = %d, TextMessage = %s, ButtonMessage = %s, MailThisToMe = %d\n", nextPage, textMessage, buttonMessage, mailThisToMe);
}

const char* MessageScreen::GetID()
{
	return "SCR_MESS";
}

UplinkObjectId MessageScreen::GetOBJECTID()
{
	return UplinkObjectId::MessageScreen;
}

void MessageScreen::SetTextMessage(const char* value)
{
	if (textMessage != nullptr)
		delete[] textMessage;

	textMessage = new char[strlen(value) + 1];
	strcpy(textMessage, value);
}

void MessageScreen::SetButtonMessage(const char* value)
{
	if (buttonMessage != nullptr)
		delete[] buttonMessage;

	buttonMessage = new char[strlen(value) + 1];
	strcpy(buttonMessage, value);
}

void MessageScreen::SetNextPage(int value)
{
	nextPage = value;
}
