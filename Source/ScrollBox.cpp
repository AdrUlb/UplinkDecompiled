#include <ScrollBox.hpp>

#include <Util.hpp>
#include <cstring>

bool ScrollBox::IsGrabInProgress()
{
	// TODO: whaaat???
	return strcmp("None", "None") != 0;
}

void ScrollBox::UpdateGrabScroll()
{
	UplinkAbort("TODO: implement ScrollBox::UpdateGrabScroll()");
}
