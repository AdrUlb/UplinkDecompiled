#include <BTree.hpp>
#include <Gci.hpp>
#include <Globals.hpp>
#include <LList.hpp>
#include <Options.hpp>
#include <csignal>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <malloc.h>

static const char* versionNumberString = "1.55";

App* app;
FILE* file_stdout;

void RunUplinkExceptionHandling()
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

void hSignalSIGSEGV(int signum)
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

void hSignalSIGFPE(int signum)
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

void hSignalSIGPIPE(int signum)
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

void Init_App(const char* exePath)
{
	(void)exePath;
	puts("TODO: implement Init_App(const char*)");
	abort();
}

char* vmg57670648335164_br_find_exe(unsigned int* error)
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

void Init_Options(int32_t argc, char** argv)
{
	(void)argc;
	(void)argv;
	puts("TODO: implement Init_Options(int, char*)");
	abort();
}

bool VerifyLegitAndCodeCardCheck()
{
	puts("TODO: implement VerifyLegitAndCodeCardCheck()");
	return true;
}

bool Load_Data()
{
	puts("TODO: implement Load_Data()");
	abort();
}

void Cleanup_Uplink()
{
	puts("TODO: implement Cleanup_Uplink()");
	abort();
}

void Init_Game()
{
	puts("TODO: implement Init_Game()");
	abort();
}

void Init_Graphics()
{
	puts("TODO: implement Init_Graphics()");
	abort();
}

void Init_OpenGL()
{
	puts("TODO: implement Init_OpenGL()");
	abort();
}

void Init_Fonts()
{
	puts("TODO: implement Init_Fonts()");
	abort();
}

void Init_Sound()
{
	puts("TODO: implement Init_Sound()");
	abort();
}

void Init_Music()
{
	puts("TODO: implement Init_Music()");
	abort();
}

void Run_MainMenu()
{
	puts("TODO: implement Run_MainMenu()");
	abort();
}
void Run_Game()
{
	puts("TODO: implement Run_Game()");
	abort();
}

void RunUplink(int argc, char** argv)
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
