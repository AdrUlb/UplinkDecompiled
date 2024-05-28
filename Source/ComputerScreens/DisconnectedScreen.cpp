#include <ComputerScreens/DisconnectedScreen.hpp>

#include <Util.hpp>

DisconnectedScreen::~DisconnectedScreen()
{
	if (textMessage != nullptr)
		delete[] textMessage;
}

bool DisconnectedScreen::Load(FILE* file)
{
	if (!ComputerScreen::Load(file))
		return false;

	if (!FileReadData(&nextPage, 4, 1, file))
		return false;

	if (!LoadDynamicString(textMessage, file))
		return false;

	return true;
}

void DisconnectedScreen::Save(FILE* file)
{
	ComputerScreen::Save(file);
	fwrite(&nextPage, 4, 1, file);
	SaveDynamicString(textMessage, file);
	SaveID_END(file);
}

void DisconnectedScreen::Print()
{
	puts("DisconnectedScreen : ");
	ComputerScreen::Print();
	printf("NextPage = %d, TextMessage = %s\n", nextPage, textMessage);
}

const char* DisconnectedScreen::GetID()
{
	return "SCR_DISC";
}

UplinkObjectId DisconnectedScreen::GetOBJECTID()
{
	return UplinkObjectId::DisconnectedScreen;
}

void DisconnectedScreen::SetTextMessage(const char* value)
{
	if (textMessage != nullptr)
		delete[] textMessage;

	textMessage = new char[strlen(value) + 1];
	strcpy(textMessage, value);
}

void DisconnectedScreen::SetNextPage(int value)
{
	nextPage = value;
}
