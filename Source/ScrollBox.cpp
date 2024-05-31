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
	// Probably unreachable
	puts("TODO: implement ScrollBox::UpdateGrabScroll()");
}

void ScrollBox::UnGrabScrollBar()
{
	// Pretty much a no-op
}
