#include <ComputerScreens/MessageScreen.hpp>

#include <Util.hpp>

MessageScreen::~MessageScreen()
{
	if (_textMessage != nullptr)
		delete[] _textMessage;

	if (_buttonMessage != nullptr)
		delete[] _buttonMessage;
}

bool MessageScreen::Load(FILE* file)
{
	if (!ComputerScreen::Load(file))
		return false;

	if (!FileReadData(&_nextPage, 4, 1, file))
		return false;

	if (!LoadDynamicString(_textMessage, file))
		return false;

	if (!LoadDynamicString(_buttonMessage, file))
		return false;

	if (!FileReadData(&_mailThisToMe, 1, 1, file))
		return false;

	return true;
}

void MessageScreen::Save(FILE* file)
{
	ComputerScreen::Save(file);
	fwrite(&_nextPage, 4, 1, file);
	SaveDynamicString(_textMessage, file);
	SaveDynamicString(_buttonMessage, file);
	fwrite(&_mailThisToMe, 1, 1, file);
}

void MessageScreen::Print()
{
	puts("MessageScreen : ");
	ComputerScreen::Print();
	printf("NextPage = %d, TextMessage = %s, ButtonMessage = %s, MailThisToMe = %d\n", _nextPage, _textMessage, _buttonMessage, _mailThisToMe);
}

const char* MessageScreen::GetID()
{
	return "SCR_MESS";
}

UplinkObjectId MessageScreen::GetOBJECTID()
{
	return UplinkObjectId::MessageScreen;
}

int MessageScreen::GetNextPage()
{
	return _nextPage;
}

const char* MessageScreen::GetTextMessage()
{
	return _textMessage;
}

const char* MessageScreen::GetButtonMessage()
{
	return _buttonMessage;
}

bool MessageScreen::GetMailThisToMe()
{
	return _mailThisToMe;
}

void MessageScreen::SetTextMessage(const char* textMessage)
{
	if (_textMessage != nullptr)
		delete[] _textMessage;

	_textMessage = new char[strlen(textMessage) + 1];
	strcpy(_textMessage, textMessage);
}

void MessageScreen::SetButtonMessage(const char* buttonMessage)
{
	if (_buttonMessage != nullptr)
		delete[] _buttonMessage;

	_buttonMessage = new char[strlen(buttonMessage) + 1];
	strcpy(_buttonMessage, buttonMessage);
}

void MessageScreen::SetNextPage(int nextPage)
{
	_nextPage = nextPage;
}
