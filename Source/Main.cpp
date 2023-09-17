#include "Main.hpp"

#include <cstdio>
#include <csignal>
#include <climits>
#include "Util.hpp"
#include "RedShirt.hpp"
#include "../UplinkDecompiledTempDefs.hpp"

static bool TestRsLoadArchive(char* fileName)
{
	if (!RsLoadArchive(fileName))
	{
		puts("\nAn error occured in Uplink");
		puts("Files integrity is not verified");
		printf("Failed loading \'%s\'\n", fileName);
		return false;
	}

	return true;
}

void hSignalSIGSEGV(int signum)
{
	puts("\nAn Uplink Internal Error has occured: segmentation violation (SIGSEGV)");
	RunUplinkExceptionHandling();
}

void hSignalSIGFPE(int signum)
{
	puts("\nAn Uplink Internal Error has occured: erroneous arithmetic operation (SIGFPE)");
	RunUplinkExceptionHandling();
}

void hSignalSIGPIPE(int signum)
{
	puts("\nAn Uplink Internal Error has occured: write to pipe with no one reading (SIGPIPE)");
	RunUplinkExceptionHandling();
}

char* vmg57670648335164_br_find_exe(int* param_1)
{
	char filePath[PATH_MAX];

	auto fileNameLength = readlink("/proc/self/exe", filePath, PATH_MAX);

	if (fileNameLength != -1)
	{
		filePath[fileNameLength] = 0;
		return strdup(filePath);
	}

	auto file = fopen("/proc/self/maps", "r");

	if (!file)
	{
		if (param_1)
			*param_1 = 1;

		return nullptr;
	}

	if (!fgets(filePath, PATH_MAX, file))
	{
		if (param_1)
			*param_1 = 2;

		fclose(file);

		return nullptr;
	}

	auto filePathLength = strlen(filePath);
	if (filePathLength != 0)
	{
		if (filePath[filePathLength - 1] == '\n')
			filePath[filePathLength - 1] = 0;

		auto filePathStart = strchr(filePath, '/');
		if (strstr(filePathStart, " r-xp ") && filePathStart)
		{
			fclose(file);
			return strdup(filePathStart);
		}
	}

	if (param_1)
		*param_1 = 3;

	fclose(file);
	return nullptr;
}


void RunUplink(int argc, char** argv)
{
	bool temp;
	char cVar1;
	const char* exeFilePath;

	if (argc >= 2 && argv[1][0] == '-' && argv[1][1] == 'v')
	{
		puts("1.55");
		return;
	}

	exeFilePath = "/opt/uk.co.introversion.uplink-full/data.dat";
	if (!DoesFileExist(exeFilePath))
		exeFilePath = vmg57670648335164_br_find_exe(0);

	Init_App(exeFilePath);
	Init_Options(argc, argv);
	cVar1 = VerifyLegitAndCodeCardCheck();
	if (cVar1 != 0) {
		temp = Load_Data();
		if (temp) {
			Init_Game();
			Init_Graphics();
			Init_OpenGL(argc, argv);
			Init_Fonts();
			Init_Sound();
			Init_Music();
			Run_MainMenu();
			Run_Game();
			Cleanup_Uplink();
			fflush(nullptr);
			return;
		}
	}
	Cleanup_Uplink();
}

int main(int argc, char* argv[])
{
	signal(SIGSEGV, hSignalSIGSEGV);
	signal(SIGFPE, hSignalSIGFPE);
	signal(SIGPIPE, hSignalSIGPIPE);
	RunUplink(argc, argv);
	return 0;
}
