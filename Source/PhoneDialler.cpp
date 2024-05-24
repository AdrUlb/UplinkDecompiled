#include <PhoneDialler.hpp>

#include <Eclipse.hpp>

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
