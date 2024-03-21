#include <BTree.hpp>
#include <Gci.hpp>
#include <Globals.hpp>
#include <LList.hpp>
#include <Options.hpp>
#include <RedShirt.hpp>
#include <csignal>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <malloc.h>

App* app = nullptr;
FILE* file_stdout = nullptr;

const char* versionNumberString = "1.55";

static char* vmg57670648335164_br_find_exe(unsigned int* error)
{
	auto buffer = (char*)malloc(0xFFF);

	if (buffer == nullptr)
	{
		if (error != nullptr)
			*error = 0;

		return nullptr;
	}

	if (buffer == nullptr)
		return nullptr;

	const auto procSelfLength = readlink("/proc/self/exe", buffer, 0xFFF);

	if (procSelfLength != -1)
	{
		buffer[procSelfLength] = 0;
		return buffer;
	}

	const auto buffer2 = (char*)realloc(buffer, 0x1080);
	if (buffer2 == nullptr)
	{
		free(buffer);
		if (error != nullptr)
			*error = 0;

		return nullptr;
	}

	const auto mapsFile = fopen("/proc/self/maps", "r");

	if (mapsFile == nullptr)
	{
		free(buffer2);
		if (error != nullptr)
			*error = 1;

		return nullptr;
	}

	if (fgets(buffer2, 0x1080, mapsFile) == 0)
	{
		fclose(mapsFile);
		free(buffer2);

		if (error != nullptr)
			*error = 2;

		return nullptr;
	}

	uint64_t mapsLen = strlen(buffer2);
	const char* slashLoc;
	const char* rxpLoc;
	if (mapsLen != 0)
	{
		// Replace newline with NULL-terminator
		if (buffer2[mapsLen - 1] == '\n')
			buffer2[mapsLen - 1] = 0;

		slashLoc = strchr(buffer2, '/');
		rxpLoc = strstr(buffer2, " r-xp ");

		if (rxpLoc != nullptr && slashLoc != nullptr)
		{
			buffer = strdup(slashLoc);
			free(buffer2);
			fclose(mapsFile);

			return buffer;
		}
	}

	fclose(mapsFile);
	free(buffer2);

	if (error != 0)
		*error = 3;

	return nullptr;
}

static void RunUplinkExceptionHandling()
{
	if (app != nullptr)
	{
		const auto options = app->GetOptionsOrNull();

		if (options != nullptr && options->GetOptionValueOrDefault("crash_graphicsinit", 0))
		{
			puts("\nAn Uplink Internal Error has occured during graphics initialization");
			if (file_stdout != nullptr)
			{
				fputs("\nAn Uplink Internal Error has occured during graphics initialization\n", file_stdout);
				fflush(file_stdout);
			}
		}
	}

	puts("\n"
		 "An (unrecognised) Uplink Internal Error has occured\n"
		 "===================================================");

	App::CoreDump();

	if (file_stdout != nullptr)
	{
		fputs("\nAn (unrecognised) Uplink Internal Error has occured\n", file_stdout);
		fputs("===================================================\n", file_stdout);
		if (app != nullptr && strcmp(app->usersPath, "c:/") != 0)
		{
			fprintf(file_stdout, "See the %sdebug.log file for more informations on the error\n", app->usersPath);
		}
		else
			fputs("See the debug.log file for more informations on the error\n", file_stdout);

		fflush(file_stdout);
	}

	GciRestoreScreenSize();
	fflush(nullptr);
	exit(0xFF);
}

static void hSignalSIGSEGV(int signum)
{
	(void)signum;
	puts("\nAn Uplink Internal Error has occured: segmentation violation (SIGSEGV)");

	if (file_stdout != nullptr)
	{
		fputs("\nAn Uplink Internal Error has occured: segmentation violation (SIGSEGV)\n", file_stdout);
		fflush(file_stdout);
	}
	return RunUplinkExceptionHandling();
}

static void hSignalSIGFPE(int signum)
{
	(void)signum;
	puts("\nAn Uplink Internal Error has occured: erroneous arithmetic operation (SIGFPE)");

	if (file_stdout != nullptr)
	{
		fputs("\nAn Uplink Internal Error has occured: erroneous arithmetic operation (SIGFPE)\n", file_stdout);
		fflush(file_stdout);
	}

	return RunUplinkExceptionHandling();
}

static void hSignalSIGPIPE(int signum)
{
	(void)signum;
	puts("\nAn Uplink Internal Error has occured: write to pipe with no one reading (SIGPIPE)");

	if (file_stdout != nullptr)
	{
		fputs("\nAn Uplink Internal Error has occured: write to pipe with no one reading (SIGPIPE)\n", file_stdout);
		fflush(file_stdout);
	}

	return RunUplinkExceptionHandling();
}

static bool VerifyLegitAndCodeCardCheck()
{
	puts("TODO: implement VerifyLegitAndCodeCardCheck()");
	return true;
}

static void Init_App(const char* exePath)
{
	char* dirPath = GetFilePath(exePath);

	app = new App();

	char buildTimeDate[0x20];
	UplinkSnprintf(buildTimeDate, sizeof(buildTimeDate), "%s at %s", __DATE__, __TIME__);
	app->Set(dirPath, versionNumberString, "RELEASE", buildTimeDate, "Uplink");

	delete[] dirPath;

	puts("=============================");
	puts("=                           =");
	puts("=        U P L I N K        =");
	puts("=                           =");
	printf("=        Version %-10s =\n", app->version);
	puts("=     - R E L E A S E -     =");
	puts("=                           =");
	puts("=============================");
	putchar('\n');
	puts(app->build);

	MakeDirectory(app->usersPath);
	MakeDirectory(app->usersTempPath);
	EmptyDirectory(app->usersTempPath);
	MakeDirectory(app->usersOldPath);

	char debugLogFile[0x100];
	UplinkSnprintf(debugLogFile, sizeof(debugLogFile), "%sdebug.log", app->usersPath);

	file_stdout = nullptr;
	int32_t fd = dup(fileno(stdout));

	if (fd != -1)
		file_stdout = fdopen(fd, "a");

	if (freopen(debugLogFile, "a", stdout) == 0)
		printf("WARNING : Failed to open %s for writing stdout\n", debugLogFile);

	if (freopen(debugLogFile, "a", stderr) == 0)
		printf("WARNING : Failed to open %s for writing stderr\n", debugLogFile);

	const auto currentTime = time(nullptr);
	const auto localTime = localtime(&currentTime);
	
	puts("\n");
	puts("===============================================");
	printf("NEW GAME     %d:%d, %d/%d/%d\n", localTime->tm_hour, localTime->tm_min, localTime->tm_mday, localTime->tm_mon + 1,
		   localTime->tm_year + 1900);
	puts("===============================================");
	printf("Version : %s\n", app->version);
	puts("RELEASE");
	puts("Linux Build");
	puts(app->build);
	printf("Path : %s\n", app->path);
	RsInitialise(app->path);
	app->Initialise();
}

static void Init_Options(int32_t argc, char** argv)
{
	(void)argc;
	(void)argv;
	UplinkAbort("TODO: implement Init_Options(int, char*)");
}

static bool Load_Data()
{
	UplinkAbort("TODO: implement Load_Data()");
}

static void Init_Game()
{
	UplinkAbort("TODO: implement Init_Game()");
}

static void Init_Graphics()
{
	UplinkAbort("TODO: implement Init_Graphics()");
}

static void Init_OpenGL()
{
	UplinkAbort("TODO: implement Init_OpenGL()");
}

static void Init_Fonts()
{
	UplinkAbort("TODO: implement Init_Fonts()");
}

static void Init_Sound()
{
	UplinkAbort("TODO: implement Init_Sound()");
}

static void Init_Music()
{
	UplinkAbort("TODO: implement Init_Music()");
}

static void Run_MainMenu()
{
	UplinkAbort("TODO: implement Run_MainMenu()");
}
static void Run_Game()
{
	UplinkAbort("TODO: implement Run_Game()");
}

static void Cleanup_Uplink()
{
	UplinkAbort("TODO: implement Cleanup_Uplink()");
}

static void RunUplink(int argc, char** argv)
{
	if (argc > 1 && argv[1][0] == '-' && argv[1][1] == 'v')
	{
		puts(versionNumberString);
		return;
	}

	const char* exePath = "/opt/uk.co.introversion.uplink-full/data.dat";

	if (!DoesFileExist(exePath))
		exePath = vmg57670648335164_br_find_exe(nullptr);

	Init_App(exePath);
	Init_Options(argc, argv);

	if (!VerifyLegitAndCodeCardCheck() || !Load_Data())
	{
		Cleanup_Uplink();
		return;
	}

	Init_Game();
	Init_Graphics();
	Init_OpenGL();
	Init_Fonts();
	Init_Sound();
	Init_Music();
	Run_MainMenu();
	Run_Game();
	Cleanup_Uplink();
	fflush(nullptr);
}

int main(int argc, char** argv)
{
	// FIXME: do not use signal
	signal(SIGSEGV, hSignalSIGSEGV);
	signal(SIGFPE, hSignalSIGFPE);
	signal(SIGPIPE, hSignalSIGPIPE);

	RunUplink(argc, argv);
	return 0;
}
