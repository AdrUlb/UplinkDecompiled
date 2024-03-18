#include <BTree.hpp>
#include <Gci.hpp>
#include <Globals.hpp>
#include <LList.hpp>
#include <Options.hpp>
#include <csignal>
#include <cstdint>
#include <cstdio>
#include <cstring>

App* app;
FILE* file_stdout;

void RunUplinkExceptionHandling()
{
	if (app != nullptr && app->OptionsValid())
	{
		const auto options = app->GetOptions();

		if (options->GetOptionValueOrDefault("crash_graphicsinit", 0))
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

void RunUplink(int argc, char* argv[])
{
	(void)argc;
	(void)argv;
	puts("TODO: implement RunUplink(int, char*)");
	abort();
}

int main(int argc, char* argv[])
{
	// FIXME: do not use signal
	signal(SIGSEGV, hSignalSIGSEGV);
	signal(SIGFPE, hSignalSIGFPE);
	signal(SIGPIPE, hSignalSIGPIPE);

	RunUplink(argc, argv);
	return 0;
}
