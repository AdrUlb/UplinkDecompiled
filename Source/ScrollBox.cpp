#include <ScrollBox.hpp>

#include <Util.hpp>
#include <cstring>

static char currentGrab[0x100] = "None";

bool ScrollBox::IsGrabInProgress()
{
	return strcmp(currentGrab, "None") != 0;
}

void ScrollBox::UpdateGrabScroll()
{
	// Probably unreachable
	puts("TODO: implement ScrollBox::UpdateGrabScroll()");
}

void ScrollBox::UnGrabScrollBar()
{
	strncpy(currentGrab, "None", 0x100);
}
