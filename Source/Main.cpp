#include "Main.hpp"

#include <cstdio>
#include <csignal>
#include <climits>
#include <ctime>
#include "Util.hpp"
#include "RedShirt.hpp"
//#include "App.hpp"
#include "../UplinkDecompiledTempDefs.hpp"
#include "../UplinkDecompiledTempGlobals.hpp"

static void hSignalSIGSEGV(int signum)
{
	puts("\nAn Uplink Internal Error has occured: segmentation violation (SIGSEGV)");
	if (gFileStdout)
	{
		fwrite("\nAn Uplink Internal Error has occured: segmentation violation (SIGSEGV)\n", 1, 72, gFileStdout);
		fflush(gFileStdout);
	}
	RunUplinkExceptionHandling();
}

static void hSignalSIGFPE(int signum)
{
	puts("\nAn Uplink Internal Error has occured: erroneous arithmetic operation (SIGFPE)");
	if (gFileStdout)
	{
		fwrite("\nAn Uplink Internal Error has occured: erroneous arithmetic operation (SIGFPE)\n", 1, 79, gFileStdout);
		fflush(gFileStdout);
	}
	RunUplinkExceptionHandling();
}

static void hSignalSIGPIPE(int signum)
{
	puts("\nAn Uplink Internal Error has occured: write to pipe with no one reading (SIGPIPE)");
	if (gFileStdout)
	{
		fwrite("\nAn Uplink Internal Error has occured: write to pipe with no one reading (SIGPIPE)\n", 1, 83, gFileStdout);
		fflush(gFileStdout);
	}
	RunUplinkExceptionHandling();
}

static char* vmg57670648335164_br_find_exe(int* errorCode)
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
		if (errorCode)
			*errorCode = 1;

		return nullptr;
	}

	if (!fgets(filePath, PATH_MAX, file))
	{
		if (errorCode)
			*errorCode = 2;

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

	if (errorCode)
		*errorCode = 3;

	fclose(file);
	return nullptr;
}

static bool TestRsLoadArchive(char* fileName)
{
	if (!RsLoadArchive(fileName))
	{
		puts("\nAn error occured in Uplink");
		puts("Files integrity is not verified");
		printf("Failed loading \'%s\'\n", fileName);
		if (gFileStdout)
		{
			puts("\nAn Uplink Error has occured");
			puts("Files integrity is not verified");
			printf("Failed loading \'%s\'\n", fileName);
			return false;
		}
		return false;
	}

	return true;
}

void Init_App(const char* exeFilePath)
{
	char* path;
	App* app;
	uint temp;
	int newStdout;
	FILE* pFVar2;
	tm* currentTimeTm;
	char debugLogFilePath[PATH_MAX];
	char buffer[32];
	time_t currentTime;

	path = GetFilePath(exeFilePath);
	app = (App*)new char[0x5c4];
	App_App(app);
	gApp = app;

	UplinkSnprintf(buffer, 0x20, "%s at %s", __DATE__, __TIME__);

	gApp->Set(path, "1.55", "RELEASE", buffer, "Uplink");

	delete[] path;

	puts("=============================");
	puts("=                           =");
	puts("=        U P L I N K        =");
	puts("=                           =");
	printf("=        Version %-10s =\n", gApp->version);
	puts("=     - R E L E A S E -     =");
	puts("=                           =");
	puts("=============================");
	putchar(10);
	puts(gApp->build);
	MakeDirectory(gApp->usersPath);
	MakeDirectory(gApp->usersTempPath);
	EmptyDirectory(gApp->usersTempPath);
	MakeDirectory(gApp->usersOldPath);
	UplinkSnprintf(debugLogFilePath, PATH_MAX, "%sdebug.log", gApp->usersPath);

	gFileStdout = nullptr;
	newStdout = dup(fileno(stdout));
	if (newStdout != -1)
		gFileStdout = fdopen(newStdout, "a");

	if (!freopen(debugLogFilePath, "a", stdout))
		printf("WARNING : Failed to open %s for writing stdout\n", debugLogFilePath);

	if (!freopen(debugLogFilePath, "a", stderr))
		printf("WARNING : Failed to open %s for writing stderr\n", debugLogFilePath);

	currentTime = time(nullptr);
	currentTimeTm = localtime(&currentTime);
	puts("\n");
	puts("===============================================");
	printf("NEW GAME     %d:%d, %d/%d/%d\n", currentTimeTm->tm_hour, currentTimeTm->tm_min,
		currentTimeTm->tm_mday, currentTimeTm->tm_mon + 1, currentTimeTm->tm_year + 1900);
	puts("===============================================");
	printf("Version : %s\n", gApp->version);
	puts("RELEASE");
	puts("Linux Build");
	puts(gApp->build);
	printf("Path : %s\n", gApp->path);
	RsInitialise(gApp->path);
	App_Initialise(gApp);
	return;
}

static void RunUplink(int argc, char** argv)
{
	if (argc >= 2 && argv[1][0] == '-' && argv[1][1] == 'v')
	{
		puts("1.55");
		return;
	}

	auto exeFilePath = "/opt/uk.co.introversion.uplink-full/data.dat";
	if (!DoesFileExist(exeFilePath))
		exeFilePath = vmg57670648335164_br_find_exe(0);

	Init_App(exeFilePath);
	Init_Options(argc, argv);

	if (!VerifyLegitAndCodeCardCheck() || !Load_Data())
	{
		Cleanup_Uplink();
		return;
	}

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
}

int main(int argc, char* argv[])
{
	signal(SIGSEGV, hSignalSIGSEGV);
	signal(SIGFPE, hSignalSIGFPE);
	signal(SIGPIPE, hSignalSIGPIPE);
	RunUplink(argc, argv);
	return 0;
}
