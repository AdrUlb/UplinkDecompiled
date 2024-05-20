#include <ExceptionHandling.hpp>
#include <Game.hpp>
#include <Globals.hpp>
#include <Opengl.hpp>
#include <RedShirt.hpp>
#include <signal.h>
#include <unistd.h>

App* app = nullptr;
FILE* file_stdout = nullptr;
const char* versionNumberString = "1.55";

Game* game = nullptr;

static float windowScaleX;
static float windowScaleY;

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

void SetWindowScaleFactor(float x, float y)
{
	windowScaleX = x;
	windowScaleY = y;
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

#ifdef NDEBUG
	int32_t fd = dup(fileno(stdout));

	if (freopen(debugLogFile, "a", stdout) == 0)
		printf("WARNING : Failed to open %s for writing stdout\n", debugLogFile);

	if (freopen(debugLogFile, "a", stderr) == 0)
		printf("WARNING : Failed to open %s for writing stderr\n", debugLogFile);

	if (fd != -1)
		file_stdout = fdopen(fd, "a");
#endif

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

static void Init_Options(int argc, char** argv)
{
	for (int i = 1; i < argc; i++)
	{
		char* arg = argv[i];
		char prefix = arg[0];
		switch (prefix)
		{
			case '+':
				app->GetOptions()->SetOptionValue(arg + 1, 1);
				break;
			case '-':
				app->GetOptions()->SetOptionValue(arg + 1, 0);
				break;
			case '!':
			{
				if (i + 1 >= argc)
				{
					printf("Error parsing command line option : %s\n", arg);
					break;
				}

				i++;

				int value;
				sscanf(argv[i], "%d", &value);
				app->GetOptions()->SetOptionValue(arg + 1, value);
				break;
			}
			default:
				printf("Error parsing command line option : %s\n", arg);
				break;
		}
	}

	if (app->GetOptions()->GetOptionValue("graphics_safemode"))
	{
		app->GetOptions()->SetOptionValue("graphics_fullscreen", 0);
		app->GetOptions()->SetOptionValue("graphics_screenrefresh", -1);
		app->GetOptions()->SetOptionValue("graphics_screendepth", -1);
		app->GetOptions()->SetOptionValue("graphics_softwaremouse", 1);
	}

	putchar('\n');

	const auto width = app->GetOptions()->GetOptionValue("graphics_screenwidth");
	const auto height = app->GetOptions()->GetOptionValue("graphics_screenheight");

	SetWindowScaleFactor(width / 640.0f, height / 480.0f);

	if (app->GetOptions()->IsOptionEqualTo("game_debugstart", 1))
	{
		puts("=====DEBUGGING INFORMATION ENABLED=====");
	}
}

static bool TestRsLoadArchive(char const* name)
{
	if (RsLoadArchive(name))
		return true;

	puts("\nAn error occured in Uplink");
	puts("Files integrity is not verified");
	printf("Failed loading '%s'\n", name);

	if (file_stdout != nullptr)
	{
		puts("\nAn Uplink Error has occured");
		puts("Files integrity is not verified");
		printf("Failed loading '%s'\n", name);
	}

	return false;
}

static bool Load_Data()
{
	const auto debugStart = app->GetOptions()->IsOptionEqualTo("game_debugstart", 1);

	if (debugStart)
		puts("Loading application data");

	if (!TestRsLoadArchive("data.dat"))
		return false;

	if (!TestRsLoadArchive("graphics.dat"))
		return false;

	if (!TestRsLoadArchive("loading.dat"))
		return false;

	if (!TestRsLoadArchive("sounds.dat"))
		return false;

	if (!TestRsLoadArchive("music.dat"))
		return false;

	if (!TestRsLoadArchive("fonts.dat"))
		return false;

	if (!TestRsLoadArchive("patch.dat"))
		return false;

	if (!TestRsLoadArchive("patch2.dat"))
		return false;

	if (!TestRsLoadArchive("patch3.dat"))
		return false;

	if (debugStart)
		puts("Finished loading application data");

	return true;
}

static void Init_Game()
{
	char debugStart = app->GetOptions()->IsOptionEqualTo("game_debugstart", 1);

	if (debugStart)
		puts("Init_Game called...creating game object");

	srand(time(nullptr));

	game = new Game();

	if (debugStart != 0)
		puts("Finished with Init_Game");
}

static void Init_Graphics()
{
	const auto options = app->GetOptions();
	options->SetThemeName(options->GetThemeName());
}

static void Init_OpenGL()
{
	app->GetOptions()->SetOptionValue("crash_graphicsinit", 1, "", true, false);
	app->GetOptions()->Save(nullptr);
	opengl_initialise();
	app->GetOptions()->SetOptionValue("crash_graphicsinit", 0, "", true, false);
	app->GetOptions()->Save(nullptr);
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
	delete app;
	app = nullptr;
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
