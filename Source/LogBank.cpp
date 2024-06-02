#include <LogBank.hpp>

#include <Util.hpp>

LogBank::~LogBank()
{
	DeleteDArrayDataD(reinterpret_cast<DArray<UplinkObject*>*>(&_accessLogs));
	DeleteDArrayDataD(reinterpret_cast<DArray<UplinkObject*>*>(&_accessLogsModified));
}

bool LogBank::Load(FILE* file)
{
	int count;
	if (!FileReadData(&count, 4, 1, file))
		return false;

	if (count > 0x40000)
	{
		printf("Print Abort: %s ln %d : ", "world/computer/logbank.cpp", 0xe9);
		printf("WARNING: LogBank::Load, number of items appears to be wrong, size=%d\n", count);
		return false;
	}

	_accessLogs.SetSize(count);
	_accessLogsModified.SetSize(count);

	for (auto i = 0; i < count; i++)
	{
		int index;
		if (!FileReadData(&index, 4, 1, file))
			return false;

		if (index != -1)
		{
			if (index >= 0x40000)
			{
				printf("Print Abort: %s ln %d : ", __FILE__, __LINE__);
				printf("WARNING: LogBank::Load, number of items appears to be wrong, index=%d\n", index);
				return false;
			}

			const auto log = new AccessLog();
			if (!log->Load(file))
			{
				delete log;
				return false;
			}
			_accessLogs.PutData(log, index);
		}

		bool modified;
		if (!FileReadData(&modified, 1, 1, file))
			return false;

		if (!modified)
		{
			if (index != -1)
				continue;

			if (!_accessLogs.ValidIndex(index))
			{
				if (_accessLogs.ValidIndex(index))
					printf("Print Assert: %s ln %d : %s\n", __FILE__, __LINE__, "!accessLogs.ValidIndex(index)");

				return false;
			}

			const auto log = new AccessLog();
			log->SetProperties(_accessLogs.GetData(index));
			_accessLogsModified.PutData(log, index);
		}
		else
		{
			const auto log = new AccessLog();
			if (!log->Load(file))
			{
				delete log;
				return false;
			}

			_accessLogsModified.PutData(log, index);
		}
	}

	return true;
}

void LogBank::Save(FILE* file)
{
	const auto count = _accessLogs.Size();
	fwrite(&count, 4, 1, file);

	for (auto i = 0; i < count; i++)
	{
		if (_accessLogs.ValidIndex(i))
		{
			fwrite(&i, 4, 1, file);
			_accessLogs.GetData(i)->Save(file);
		}
		else
		{
			const int buf = -1;
			fwrite(&buf, 4, 1, file);
		}

		const auto modified = LogModified(i);
		fwrite(&modified, 1, 1, file);
		if (modified)
		{
			UplinkAssert(_accessLogsModified.ValidIndex(i));
			_accessLogsModified.GetData(i)->Save(file);
		}
	}
}

void LogBank::Print()
{
	puts("LogBank : ");
	PrintDArray(reinterpret_cast<DArray<UplinkObject*>*>(&_accessLogs));
	PrintDArray(reinterpret_cast<DArray<UplinkObject*>*>(&_accessLogsModified));
}

const char* LogBank::GetID()
{
	return "LOGBANK";
}

UplinkObjectId LogBank::GetOBJECTID()
{
	return UplinkObjectId::LogBank;
}

void LogBank::AddLog(AccessLog* log, int index)
{
	if (index == -1)
		index = _accessLogs.Size();

	if (index >= _accessLogs.Size())
		_accessLogs.SetSize(index + 1);

	_accessLogs.PutData(log, index);

	struct AccessLog* log2 = new AccessLog();
	log2->SetProperties(log);

	_accessLogsModified.SetSize(_accessLogs.Size());
	_accessLogsModified.PutData(log2, index);
}

bool LogBank::LogModified(int index)
{
	if (!_accessLogs.ValidIndex(index))
		return false;

	if (!_accessLogsModified.ValidIndex(index))
		return false;

	const auto log = _accessLogs.GetData(index);
	const auto logModified = _accessLogsModified.GetData(index);

	if (log->GetTYPE() != logModified->GetTYPE())
		return true;

	if (log->GetSuspicious() != logModified->GetSuspicious())
		return true;

	if (strcmp(log->GetFromIp(), logModified->GetFromIp()) != 0)
		return true;

	if (strcmp(log->GetFromName(), logModified->GetFromName()) != 0)
		return true;

	if (!log->GetDate().Equal(logModified->GetDate()))
		return true;

	return false;
}
