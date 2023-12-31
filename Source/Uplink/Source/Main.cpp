#include <cstdio>
#include <csignal>
#include <climits>
#include <ctime>
#include <cstring>
#include <Util.hpp>
#include <RedShirt.hpp>
#include <Sound.hpp>
#include <Graphics.hpp>
#include "../TempDefs.hpp"
#include "../TempGlobals.hpp"

static void RunUplinkExceptionHandling()
{
	if (gApp)
	{
		const auto options = gApp->GetOptionsOrNull();
		if (options)
		{
			if (options->GetOption("crash_graphicsinit") && options->GetOptionValue("crash_graphicsinit"))
			{
				puts("\nAn Uplink Internal Error has occured during graphics initialization");
				if (gFileStdout)
				{
					fputs("\nAn Uplink Internal Error has occured during graphics initialization\n", gFileStdout);
					fflush(gFileStdout);
				}
			}
		}
	}
	puts("\nAn (unrecognised) Uplink Internal Error has occured");
	puts("===================================================");
	App::CoreDump();
	if (gFileStdout)
	{
		fputs("\nAn (unrecognised) Uplink Internal Error has occured\n", gFileStdout);
		fputs("===================================================\n", gFileStdout);
		if (!gApp)
		{
			fputs("See the debug.log file for more informations on the error\n", gFileStdout);
		}
		else
		{
			if (strcmp("c:/", gApp->UsersPath) == 0)
				fputs("See the debug.log file for more informations on the error\n", gFileStdout);
			else
				fprintf(gFileStdout, "See the %sdebug.log file for more informations on the error\n", gApp->UsersPath);
		}
		fflush(gFileStdout);
	}

	GciRestoreScreenSize();
	fflush(nullptr);
	exit(0xFF);
}

static void SignalHandler(int signum)
{
	const auto sigstr = sigdescr_np(signum);
	const auto sigabbrev = sigabbrev_np(signum);
	printf("\nAn Uplink Internal Error has occured: %s (%s)", sigstr, sigabbrev);
	if (gFileStdout)
	{
		fprintf(gFileStdout, "\nAn Uplink Internal Error has occured: %s (%s)\n", sigstr, sigabbrev);
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

static bool TestRsLoadArchive(const char* fileName)
{
	if (!RsLoadArchive(fileName))
	{
		puts("\nAn error occured in Uplink");
		puts("Files integrity is not verified");
		printf("Failed loading \'%s\'\n", fileName);
		if (gFileStdout)
		{
			fputs("\nAn Uplink Error has occured", gFileStdout);
			fputs("Files integrity is not verified", gFileStdout);
			fprintf(gFileStdout, "Failed loading \'%s\'\n", fileName);
			return false;
		}
		return false;
	}

	return true;
}

static void SetWindowScaleFactor(float x, float y)
{
	gWindowScaleX = x;
	gWindowScaleY = y;
}

static void Init_App(const char* exeFilePath)
{
	gApp = new App();

	char buildDateTime[0x20];
	UplinkSnprintf(buildDateTime, 0x20, "%s at %s", __DATE__, __TIME__);

	auto gameDirPath = GetFilePath(exeFilePath);
	gApp->Set(gameDirPath, "1.55", "RELEASE", buildDateTime, "Uplink");
	delete[] gameDirPath;

	puts("=============================");
	puts("=                           =");
	puts("=        U P L I N K        =");
	puts("=                           =");
	printf("=        Version %-10s =\n", gApp->Version);
	puts("=     - R E L E A S E -     =");
	puts("=                           =");
	puts("=============================");
	putchar(10);
	puts(gApp->Build);
	MakeDirectory(gApp->UsersPath);
	MakeDirectory(gApp->UsersTempPath);
	EmptyDirectory(gApp->UsersTempPath);
	MakeDirectory(gApp->UsersOldPath);

	char debugLogFilePath[PATH_MAX];
	UplinkSnprintf(debugLogFilePath, PATH_MAX, "%sdebug.log", gApp->UsersPath);

	auto newStdout = dup(fileno(stdout));
	if (newStdout != -1)
		gFileStdout = fdopen(newStdout, "a");
	else
		gFileStdout = nullptr;

	/*if (!freopen(debugLogFilePath, "a", stdout))
		printf("WARNING : Failed to open %s for writing stdout\n", debugLogFilePath);

	if (!freopen(debugLogFilePath, "a", stderr))
		printf("WARNING : Failed to open %s for writing stderr\n", debugLogFilePath);*/

	auto currentTime = time(nullptr);
	auto currentTimeTm = localtime(&currentTime);
	puts("\n");
	puts("===============================================");
	printf("NEW GAME     %d:%d, %d/%d/%d\n", currentTimeTm->tm_hour, currentTimeTm->tm_min,
		currentTimeTm->tm_mday, currentTimeTm->tm_mon + 1, currentTimeTm->tm_year + 1900);
	puts("===============================================");
	printf("Version : %s\n", gApp->Version);
	puts("RELEASE");
	puts("Linux Build");
	puts(gApp->Build);
	printf("Path : %s\n", gApp->Path);
	RsInitialise(gApp->Path);
	gApp->Initialise();
}

static void Init_Options(int argc, char* argv[])
{
	const auto options = gApp->GetOptions();

	for (auto i = 1; i < argc; i++)
	{
		const auto arg = argv[i];
		const auto mode = arg[0];

		if (mode == 0)
		{
			printf("Error parsing command line option : %s\n", arg);
			continue;
		}

		const auto name = arg + 1;
		switch (mode)
		{
		case '+':
			options->SetOptionValue(name, 1);
			break;
		case '-':
			options->SetOptionValue(name, 0);
			break;
		case '!':
			if (++i >= argc)
			{
				printf("Error parsing command line option : %s\n", arg);
				break;
			}

			int value;
			sscanf(argv[i], "%d", &value);
			options->SetOptionValue(name, value);
			break;
		default:
			printf("Error parsing command line option : %s\n", arg);
			break;
		}
	}

	if (options->GetOptionValue("graphics_safemode") == 1)
	{
		options->SetOptionValue("graphics_fullscreen", 0);
		options->SetOptionValue("graphics_screenrefresh", -1);
		options->SetOptionValue("graphics_screendepth", -1);
		options->SetOptionValue("graphics_softwaremouse", 1);
	}

	putchar('\n');

	const auto width = options->GetOptionValue("graphics_screenwidth");
	const auto height = options->GetOptionValue("graphics_screenheight");
	SetWindowScaleFactor(width / 640.0f, height / 480.0f);

	if (options->IsOptionEqualTo("game_debugstart", 1))
		puts("=====DEBUGGING INFORMATION ENABLED=====");
	return;
}

static bool Load_Data()
{
	const auto debugStart = gApp->GetOptions()->IsOptionEqualTo("game_debugstart", 1);

	if (debugStart != 0)
		puts("Loading application data");

	if (TestRsLoadArchive("data.dat") &&
		TestRsLoadArchive("graphics.dat") &&
		TestRsLoadArchive("loading.dat") &&
		TestRsLoadArchive("sounds.dat") &&
		TestRsLoadArchive("music.dat") &&
		TestRsLoadArchive("fonts.dat") &&
		TestRsLoadArchive("patch.dat") &&
		TestRsLoadArchive("patch2.dat") &&
		TestRsLoadArchive("patch3.dat"))
	{
		if (debugStart != 0)
			puts("Finished loading application data");

		return true;
	}

	return false;
}

static void Init_Game()
{
	const auto debug = gApp->GetOptions()->IsOptionEqualTo("game_debugstart", 1);

	if (debug)
		puts("Init_Game called...creating game object");

	srand(time(nullptr));

	gGame = new Game();

	if (debug)
		puts("Finished with Init_Game");
}

static void Init_Graphics()
{
	const auto options = gApp->GetOptions();
	options->SetThemeName(options->GetThemeName());
}

static void Init_OpenGL(int argc, char* argv[])
{
	const auto options = gApp->GetOptions();

	options->SetOptionValue("crash_graphicsinit", 1, "", true, false);
	options->Save(nullptr);
	
	opengl_initialise(argc, argv);

	options->SetOptionValue("crash_graphicsinit", 0, "", true, false);
	options->Save(nullptr);
}

static void Init_Sound()
{
	const auto debug = gApp->GetOptions()->IsOptionEqualTo("game_debugstart", 1);
	if (debug)
		puts("Init_Sound called...setting up sound system");

	SgInitialise();

	if (debug)
		puts("Finished with Init_Sound");
}

static void Cleanup_Uplink()
{
	if (gApp)
		delete gApp;
}

static void RunUplink(int argc, char* argv[])
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
	signal(SIGSEGV, SignalHandler);
	signal(SIGFPE, SignalHandler);
	signal(SIGPIPE, SignalHandler);
	RunUplink(argc, argv);
	return 0;
}
