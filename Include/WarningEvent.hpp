#include <UplinkEvent.hpp>

class WarningEvent : public UplinkEvent
{
	UplinkEvent* _event = nullptr;

	bool Load(FILE* file) override;
	void Save(FILE* file) override;
	void Print() override;
	const char* GetID() override;
	UplinkObjectId GetOBJECTID() override;
	void Run() override;
	char* GetShortString() override;
	char* GetLongString() override;
};
