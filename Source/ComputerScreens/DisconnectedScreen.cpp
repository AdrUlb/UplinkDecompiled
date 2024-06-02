#include <ComputerScreens/DisconnectedScreen.hpp>

#include <Util.hpp>

DisconnectedScreen::~DisconnectedScreen()
{
	if (_textMessage != nullptr)
		delete[] _textMessage;
}

bool DisconnectedScreen::Load(FILE* file)
{
	if (!ComputerScreen::Load(file))
		return false;

	if (!FileReadData(&_nextPage, 4, 1, file))
		return false;

	if (!LoadDynamicString(_textMessage, file))
		return false;

	return true;
}

void DisconnectedScreen::Save(FILE* file)
{
	ComputerScreen::Save(file);
	fwrite(&_nextPage, 4, 1, file);
	SaveDynamicString(_textMessage, file);
}

void DisconnectedScreen::Print()
{
	puts("DisconnectedScreen : ");
	ComputerScreen::Print();
	printf("NextPage = %d, TextMessage = %s\n", _nextPage, _textMessage);
}

const char* DisconnectedScreen::GetID()
{
	return "SCR_DISC";
}

UplinkObjectId DisconnectedScreen::GetOBJECTID()
{
	return UplinkObjectId::DisconnectedScreen;
}

void DisconnectedScreen::SetTextMessage(const char* textMessage)
{
	if (_textMessage != nullptr)
		delete[] textMessage;

	_textMessage = new char[strlen(textMessage) + 1];
	strcpy(_textMessage, textMessage);
}

void DisconnectedScreen::SetNextPage(int nextPage)
{
	_nextPage = nextPage;
}
