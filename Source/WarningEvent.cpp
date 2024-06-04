#include <WarningEvent.hpp>

#include <Util.hpp>
#include <cstring>

bool WarningEvent::Load(FILE* file)
{
	_event = nullptr;

	if (!UplinkEvent::Load(file))
		return false;

	bool hasEvent;
	if (!FileReadData(&hasEvent, 1, 1, file))
		return false;

	if (hasEvent)
	{
		UplinkObjectId objectId;
		if (!FileReadData(&objectId, 4, 1, file))
			return false;

		_event = dynamic_cast<UplinkEvent*>(CreateUplinkObject(objectId));
		if (_event == nullptr)
			return false;

		if (!_event->Load(file))
			return false;
	}

	return true;
}

void WarningEvent::Save(FILE* file)
{
	UplinkEvent::Save(file);

	const auto hasEvent = this->_event != nullptr;

	fwrite(&hasEvent, 1, 1, file);

	if (hasEvent)
	{
		const auto objectId = _event->GetOBJECTID();
		fwrite(&objectId, 4, 1, file);
		_event->Save(file);
	}
}

void WarningEvent::Print()
{
	puts("Warning Event : ");
	UplinkEvent::Print();

	if (_event == nullptr)
		return;

	return _event->Print();
}

const char* WarningEvent::GetID()
{
	return "EVT_WARN";
}

UplinkObjectId WarningEvent::GetOBJECTID()
{
	return UplinkObjectId::WarningEvent;
}

void WarningEvent::Run()
{
	if (_event == nullptr)
		return;

	_event->RunWarning();
}

char* WarningEvent::GetShortString()
{
	const auto str = "Warning Event";
	const auto ret = new char[strlen(str) + 1];
	sprintf(ret, "%s", str);
	return ret;
}

char* WarningEvent::GetLongString()
{
	const auto format = "Warning Event for :\n";
	const auto str = _event->GetLongString();

	const auto formatLength = strlen(format);

	const auto ret = new char[formatLength + strlen(str) + 1];

	sprintf(ret, "%s", format);
	sprintf(ret + formatLength, "%s", str);
	return ret;
}
