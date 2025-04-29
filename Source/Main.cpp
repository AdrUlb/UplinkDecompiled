#include <ExceptionHandling.hpp>
#include <FTGL/ftgl.h>
#include <Game.hpp>
#include <Gci.hpp>
#include <Globals.hpp>
#include <Opengl.hpp>
#include <RedShirt.hpp>
#include <ScriptLibrary.hpp>
#include <Sg.hpp>
#include <signal.h>
#include <sys/stat.h>
#include <unistd.h>
#include FT_DRIVER_H
#include FT_MODULE_H

App* app = nullptr;
FILE* file_stdout = nullptr;
const char* versionNumberString = "1.55";
Game* game = nullptr;
const char* minSaveVersion = "SAV56";
const char* latestSaveVersion = "SAV62";
char tempname[0x80] = { 0 };

static char* vmg57670648335164_br_find_exe(unsigned int* error)
{
	const auto buffer = new char[0x1080];

	if (buffer == nullptr)
	{
		if (error != nullptr)
			*error = 0;

		return nullptr;
	}

	const auto procSelfLength = readlink("/proc/self/exe", buffer, 0xFFF);

	if (procSelfLength != -1)
	{
		buffer[procSelfLength] = 0;
		return buffer;
	}

	const auto mapsFile = fopen("/proc/self/maps", "r");

	if (mapsFile == nullptr)
	{
		delete[] buffer;
		if (error != nullptr)
			*error = 1;

		return nullptr;
	}

	if (fgets(buffer, 0x1080, mapsFile) == 0)
	{
		fclose(mapsFile);
		delete[] buffer;

		if (error != nullptr)
			*error = 2;

		return nullptr;
	}

	uint64_t mapsLen = strlen(buffer);
	const char* slashLoc;
	const char* rxpLoc;
	if (mapsLen != 0)
	{
		// Replace newline with NULL-terminator
		if (buffer[mapsLen - 1] == '\n')
			buffer[mapsLen - 1] = 0;

		slashLoc = strchr(buffer, '/');
		rxpLoc = strstr(buffer, " r-xp ");

		if (rxpLoc != nullptr && slashLoc != nullptr)
		{
			const auto buf = strdup(slashLoc);
			delete[] buffer;
			fclose(mapsFile);
			return buf;
		}
	}

	fclose(mapsFile);
	delete[] buffer;

	if (error != 0)
		*error = 3;

	return nullptr;
}

static bool VerifyLegitAndCodeCardCheck()
{
	char worldDatFilePath[0x100];
	UplinkSnprintf(worldDatFilePath, 0x100, "%sworld.dat", app->Path);

	if (!DoesFileExist(worldDatFilePath))
	{
		const auto worldDatFilePathLength = strlen(worldDatFilePath);
		auto i = worldDatFilePathLength - 1;
		for (; (i + 1) != 0 && worldDatFilePath[i] != '\\' && worldDatFilePath[i] != '/'; i--)
			;

		if (i >= 4)
		{
			const auto c0 = worldDatFilePath[i - 4];
			const auto c1 = worldDatFilePath[i - 3];
			const auto c2 = worldDatFilePath[i - 2];
			const auto c3 = worldDatFilePath[i - 1];
			const auto c4 = worldDatFilePath[i];

			if ((c0 == '\\' || c0 == '/') && (c1 == 'l' || c1 == 'L') && (c2 == 'i' || c2 == 'I') && (c3 == 'b' || c3 == 'B') && (c4 == '\\' || c4 == '/'))
			{
				for (auto j = i + 1; j <= worldDatFilePathLength; j++)
					worldDatFilePath[j - 4] = worldDatFilePath[j];
			}
		}
	}

	static constexpr unsigned char magic1[9]{ 0xE7, 0x6B, 0x7E, 0x6B, 0x4C, 0x4F, 0x57, 0x7D, 0x00 };
	static constexpr unsigned char magic2[9]{ 0xAA, 0xAB, 0x15, 0xDD, 0xDD, 0xEE, 0xE9, 0x2D, 0x00 };
	static constexpr unsigned char magic3[9]{ 0xC1, 0xEC, 0xD6, 0x8B, 0x02, 0x07, 0x28, 0xE8, 0x00 };

	char magicReadBuf[9];
	const auto file = fopen(worldDatFilePath, "rb");

	if (file == nullptr)
		goto returnError;

	fseek(file, 0x80, 0);
	fread(magicReadBuf, 8, 1, file);
	magicReadBuf[8] = 0;

	struct stat worldDatStat;

	if (strcmp(magicReadBuf, reinterpret_cast<const char*>(magic1)) != 0)
		goto returnError;

	stat(worldDatFilePath, &worldDatStat);
	if (worldDatStat.st_size != 14400792)
		goto returnError;

	fseek(file, 0x100, 0);
	fread(magicReadBuf, 8, 1, file);
	magicReadBuf[8] = 0;

	if (strcmp(magicReadBuf, reinterpret_cast<const char*>(magic2)) == 0)
		goto returnError;

	fseek(file, 0x240, 0);
	fread(magicReadBuf, 8, 1, file);
	magicReadBuf[8] = 0;

	if (strcmp(magicReadBuf, reinterpret_cast<const char*>(magic3)) == 0)
		app->SetCheckCodeCard(false);

	return true;

returnError:
	fclose(file);

	puts("\nAn error occured in Uplink");
	puts("Files integrity is not verified");

	if (!file_stdout)
		return false;

	puts("\nAn Uplink Error has occured");
	puts("Files integrity is not verified");
	return false;
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
	printf("=        Version %-10s =\n", app->Version);
	puts("=     - R E L E A S E -     =");
	puts("=                           =");
	puts("=============================");
	putchar('\n');
	puts(app->Build);

	MakeDirectory(app->UsersPath);
	MakeDirectory(app->UsersTempPath);
	EmptyDirectory(app->UsersTempPath);
	MakeDirectory(app->UsersOldPath);

	char debugLogFile[0x100];
	UplinkSnprintf(debugLogFile, sizeof(debugLogFile), "%sdebug.log", app->UsersPath);

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
	printf("NEW GAME     %d:%d, %d/%d/%d\n", localTime->tm_hour, localTime->tm_min, localTime->tm_mday, localTime->tm_mon + 1, localTime->tm_year + 1900);
	puts("===============================================");
	printf("Version : %s\n", app->Version);
	puts("RELEASE");
	puts("Linux Build");
	puts(app->Build);
	printf("Path : %s\n", app->Path);
	RsInitialise(app->Path);
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
			app->GetOptions().SetOptionValue(arg + 1, 1);
			break;
		case '-':
			app->GetOptions().SetOptionValue(arg + 1, 0);
			break;
		case '!': {
			if (i + 1 >= argc)
			{
				printf("Error parsing command line option : %s\n", arg);
				break;
			}

			i++;

			int value;
			sscanf(argv[i], "%d", &value);
			app->GetOptions().SetOptionValue(arg + 1, value);
			break;
		}
		default:
			printf("Error parsing command line option : %s\n", arg);
			break;
		}
	}

	if (app->GetOptions().GetOptionValue("graphics_safemode"))
	{
		app->GetOptions().SetOptionValue("graphics_fullscreen", 0);
		app->GetOptions().SetOptionValue("graphics_screenrefresh", -1);
		app->GetOptions().SetOptionValue("graphics_screendepth", -1);
		app->GetOptions().SetOptionValue("graphics_softwaremouse", 1);
	}

	putchar('\n');

	const auto width = app->GetOptions().GetOptionValue("graphics_screenwidth");
	const auto height = app->GetOptions().GetOptionValue("graphics_screenheight");

	SetWindowScaleFactor(width / 640.0f, height / 480.0f);

	if (app->GetOptions().IsOptionEqualTo("game_debugstart", 1))
	{
		puts("=====DEBUGGING INFORMATION ENABLED=====");
	}
}

static bool TestRsLoadArchive(const char* name)
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
	const auto debugStart = app->GetOptions().IsOptionEqualTo("game_debugstart", 1);

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
	char debugStart = app->GetOptions().IsOptionEqualTo("game_debugstart", 1);

	if (debugStart)
		puts("Init_Game called...creating game object");

	srand(time(nullptr));

	game = new Game();

	if (debugStart != 0)
		puts("Finished with Init_Game");
}

static void Init_Graphics()
{
	auto& options = app->GetOptions();
	options.SetThemeName(options.GetThemeName());
}

static void Init_OpenGL()
{
	app->GetOptions().SetOptionValue("crash_graphicsinit", 1, "", true, false);
	app->GetOptions().Save(nullptr);
	opengl_initialise();
	app->GetOptions().SetOptionValue("crash_graphicsinit", 0, "", true, false);
	app->GetOptions().Save(nullptr);
}

static void Init_Fonts()
{
	FT_UInt interpreterVersion = TT_INTERPRETER_VERSION_35;
#ifndef UPLINKDECOMPILEDPRELOADED
	FT_Property_Set(*FTLibrary::Instance().GetLibrary(), "truetype", "interpreter-version", &interpreterVersion);
#endif
	const auto debug = app->GetOptions().IsOptionEqualTo("game_debugstart", 1);

	if (debug)
		puts("Init_Fonts called...setting up system fonts");

	GciEnableTrueTypeSupport();

	const auto path = RsArchiveFileOpen("fonts/dungeon.ttf");

	if (path == nullptr)
	{
		printf("Unable to open font : %s\n", "fonts/dungeon.ttf");
		GciDisableTrueTypeSupport();
		puts("True type font support is DISABLED");
	}
	else
	{
		if (debug)
		{
			printf("Registering fonts...");
			printf("done\n ");
			puts("Loading system fonts into memory...");
		}

		const auto success =
			GciLoadTrueTypeFont(7, "Dungeon", path, 16) && GciLoadTrueTypeFont(5, "Dungeon", path, 9) && GciLoadTrueTypeFont(6, "Dungeon", path, 11);

		if (debug)
		{
			puts("done");
			printf("Unregistering fonts...");
			printf("done\n ");
		}

		if (!success)
		{
			GciDisableTrueTypeSupport();
			puts("True type font support is DISABLED");
		}
	}

	GciSetDefaultFont(5);

	if (debug)
		printf("Finished with Init_Fonts\n ");
}

static void Init_Sound()
{
	const auto debug = app->GetOptions().IsOptionEqualTo("game_debugstart", 1);

	if (debug)
		puts("Init_Sound called...setting up sound system");

	SgInitialise();

	if (debug)
		puts("Finished with Init_Sound");
}

static void Init_Music()
{
	const auto debug = app->GetOptions().IsOptionEqualTo("game_debugstart", 1);

	if (debug)
		puts("Init_Music called...loading modules");

	SgPlaylist_Initialise();
	SgSetModVolume(20);
	SgPlaylist_Create("main");
	SgPlaylist_AddSong("main", "music/bluevalley.uni");
	SgPlaylist_AddSong("main", "music/serenity.uni");
	SgPlaylist_AddSong("main", "music/mystique.uni");
	SgPlaylist_AddSong("main", "music/a94final.uni");
	SgPlaylist_AddSong("main", "music/symphonic.uni");
	SgPlaylist_AddSong("main", "music/myst2.uni");

	if (debug)
		puts("Finished with Init_Music");
}

static void Run_MainMenu()
{
	if (app->GetOptions().IsOptionEqualTo("game_debugstart", 1))
	{
		puts("Creating main menu.");
		puts("====== END OF DEBUGGING INFORMATION ====");
	}
	float currentVersion;
	sscanf(versionNumberString, "%f", &currentVersion);
	currentVersion *= 100.0f;
	const auto prevVersion = app->GetOptions().GetOptionValue("game_version");
	if (prevVersion != currentVersion)
	{
		puts("New patch Detected!");
		printf("Old version = %d\n", prevVersion);
		printf("New version = %d\n\n", static_cast<int>(currentVersion));

		app->GetOptions().SetOptionValue("game_version", currentVersion, "z", false, false);

		if (prevVersion <= 119)
		{
			app->GetMainMenu().RunScreen(MainMenuScreenCode::FirstTimeLoading);
			ScriptLibrary::RunScript(45);
			return;
		}
	}
	else if (app->GetOptions().IsOptionEqualTo("game_firsttime", 1))
	{
		app->GetMainMenu().RunScreen(MainMenuScreenCode::FirstTimeLoading);
		GciTimerFunc(2000, ScriptLibrary::RunScript, 30);
	}
	else
		app->GetMainMenu().RunScreen(MainMenuScreenCode::Login);
}

static void Run_Game()
{
	opengl_run();
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

	auto exePath = "/opt/uk.co.introversion.uplink-full/data.dat";

	if (!DoesFileExist(exePath))
	{
		exePath = vmg57670648335164_br_find_exe(nullptr);
		Init_App(exePath);
		delete[] exePath;
	}
	else
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
	// Avoid some graphical glitches (no idea why this happens but it also happens with the original game)
	setenv("LIBGL_ALWAYS_SOFTWARE", "1", 1);

	// FIXME: do not use signal
	signal(SIGSEGV, hSignalSIGSEGV);
	signal(SIGFPE, hSignalSIGFPE);
	signal(SIGPIPE, hSignalSIGPIPE);

	RunUplink(argc, argv);
	return 0;
}

#ifdef UPLINKDECOMPILEDPRELOADED
#include <sys/mman.h>

uintptr_t PAGESIZE = 0;

constexpr uintptr_t uplinkMainAddr = 0x004A12B0;

static inline uintptr_t GetPageFromAddress(uintptr_t address)
{
	return address & ~(PAGESIZE - 1);
}
template <typename T> static void Write(uintptr_t address, T value)
{
	const auto ptr = (T*)address;

	const auto page1 = GetPageFromAddress(address);
	const auto page2 = GetPageFromAddress(address + sizeof(T) - 1);

	mprotect((void*)page1, PAGESIZE, PROT_WRITE);

	if (page1 != page2)
		mprotect((void*)page2, PAGESIZE, PROT_WRITE);

	*ptr = value;

	mprotect((void*)page1, PAGESIZE, PROT_EXEC | PROT_READ);

	if (page1 != page2)
		mprotect((void*)page2, PAGESIZE, PROT_EXEC | PROT_READ);
}

static void WriteJump(uintptr_t from, uintptr_t to)
{
	Write<uint8_t>(from++, 0xFF);
	Write<uint8_t>(from++, 0x25);
	Write<uint8_t>(from++, 0x00);
	Write<uint8_t>(from++, 0x00);
	Write<uint8_t>(from++, 0x00);
	Write<uint8_t>(from++, 0x00);
	Write<uint64_t>(from, to);
}

__attribute__((constructor)) void LibMain()
{
	PAGESIZE = sysconf(_SC_PAGE_SIZE);

	WriteJump(uplinkMainAddr, (uintptr_t)main);
}
#endif
