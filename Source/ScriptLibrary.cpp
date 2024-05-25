#include <ScriptLibrary.hpp>

void ScriptLibrary::RunScript(int index)
{
	switch (index)
	{
		default:
			printf("WARNING: Unknown script index %d\n", index);
			UplinkAbort("TODO: Script not implemented!!");
			break;
	}
}
