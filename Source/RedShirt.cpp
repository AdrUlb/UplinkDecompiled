#include <RedShirt.hpp>
#include <Util.hpp>
#include <cstdio>
#include <cstdlib>

static char rsapppath[0x100];
static char tempdir[0x100];

static bool rsInitialised = false;

void RsCleanUp()
{
	UplinkAbort("TODO: implement RsCleanUp()");
}

void RsInitialise(const char* appPath)
{
	strcpy(rsapppath, appPath);
	strncpy(tempdir, "/tmp/uplink-XXXXXX", 19);

	if (mkdtemp(tempdir) == 0)
	{
		puts("Failed to make temporary directory");
		abort();
	}

	uint64_t tempdirLength = strlen(tempdir);

	rsInitialised = true;

	tempdir[tempdirLength] = '/';
	tempdir[tempdirLength + 1] = 0;

	atexit(RsCleanUp);
}
