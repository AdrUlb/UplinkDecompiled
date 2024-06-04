#include <Eclipse.hpp>
#include <GL/gl.h>
#include <Gci.hpp>
#include <Globals.hpp>
#include <NameGenerator.hpp>
#include <Opengl.hpp>
#include <RedShirt.hpp>
#include <ScriptLibrary.hpp>
#include <Sg.hpp>

#if 0
#define LOCALHOST_IMAGE "start/localhost_linux.tif"
#else
#define LOCALHOST_IMAGE "start/localhost_win32.tif"
#endif

static void DrawConnection(Button* button, bool highlighted, bool clicked)
{
	(void)highlighted;
	(void)clicked;
	const auto screenHeight = app->GetOptions()->GetOptionValue("graphics_screenheight");

	glScissor(button->X, screenHeight - button->Y - button->Height, button->Width, button->Height);
	glEnable(GL_SCISSOR_TEST);
	glColor4f(0.3f, 0.5f, 1.0f, 1.0f);
	glLineWidth(2.0f);

	if (button->Height == 4)
	{
		glBegin(GL_LINES);
		glVertex2i(button->X + 1, button->Y + 1);
		glVertex2i(button->X + button->Width, button->Y + 1);
		glEnd();
	}
	else if (button->Width == 4)
	{
		glBegin(GL_LINES);
		glVertex2i(button->X + 1, button->Y + 1);
		glVertex2i(button->X + 1, button->Height + button->Y - 1);
		glEnd();
	}
	else
	{
		glBegin(GL_LINES);
		glVertex2i(button->X + 1, button->Y + 1);
		glVertex2i(button->Width + button->X - 1, button->Height + button->Y - 1);
		glEnd();
	}

	glLineWidth(1.0f);
	glDisable(GL_SCISSOR_TEST);
}

static void Script30()
{
	EclRegisterCaptionChange("firsttimeloading_text", "Establishing connection to UPLINK public access server...", 1500, nullptr);
	GciTimerFunc(1500, ScriptLibrary::RunScript, 31);
}

static void Script31()
{
	if (EclGetButton("start_localhost") == nullptr)
	{
		EclRegisterButton(230, 200, 64, 78, " ", " ", "start_localhost");
		button_assignbitmap("start_localhost", LOCALHOST_IMAGE);
		EclRegisterButton(330, 220, 32, 32, " ", " ", "start_key");
		button_assignbitmap("start_key", "analyser/nonsecure.tif");
		const auto buttonStartKey = EclGetButton("start_key");

		UplinkAssert(buttonStartKey != nullptr);

		EclRegisterButton(buttonStartKey->X - 43, buttonStartKey->Y - 1 + (buttonStartKey->Height / 2), 42, 4, " ", " ", "start_keylink");
		EclRegisterButtonCallbacks("start_keylink", DrawConnection, nullptr, nullptr, nullptr);
	}
	else
	{
		if (EclGetButton("start_target") != nullptr)
		{
			const auto dialler = new PhoneDialler();
			dialler->DialNumber(400, 170, "234.773.0.666", PhoneDiallerNextScene::Script92, nullptr);
			return;
		}

		EclRegisterButton(690, 320, 100, 100, " ", " ", "start_target");
		button_assignbitmap("start_target", "start/publicaccessserver.tif");
		EclRegisterButton(650, 340, 32, 32, " ", " ", "start_lock");
		button_assignbitmap("start_lock", "analyser/secure.tif");

		const auto startLockButton = EclGetButton("start_lock");
		UplinkAssert(startLockButton != nullptr);

		EclRegisterButton(startLockButton->Width + 450 + 200, startLockButton->Height / 2 + 339, 31, 4, " ", " ", "start_locklink");
		EclRegisterButtonCallbacks("start_locklink", DrawConnection, nullptr, nullptr, nullptr);
		EclRegisterMovement("start_target", 490, 320, 500, nullptr);
		EclRegisterMovement("start_lock", 450, 340, 500, nullptr);
		EclRegisterMovement("start_locklink", startLockButton->Width + 450, (startLockButton->Height / 2) + 339, 500, nullptr);
		SgPlaySound(RsArchiveFileOpen("sounds/newmail.wav"), "sounds/newmail.wav");
	}

	return GciTimerFunc(800, ScriptLibrary::RunScript, 31);
}

static void Script32()
{
	game->NewGame();
	SgPlaySound(RsArchiveFileOpen("sounds/phonepickup.wav"), "sounds/phonepickup.wav");
	EclRemoveButton("start_localhost");
	EclRemoveButton("start_key");
	EclRemoveButton("start_keylink");
	EclRemoveButton("start_lock");
	EclRemoveButton("start_locklink");
	EclRemoveButton("start_link");
	app->GetMainMenu()->Remove();
}

static void Script33()
{
	if (strcmp(game->GetWorld().GetPlayer().GetHandle(), "NEWAGENT") != 0)
	{
		create_msgbox("Error", "Our records show you are already\nregistered as an Uplink agent.", nullptr);
		return;
	}

	char name[0x40];
	char password[0x21];
	char password2[0x21];
	char accessCode[0x80];

	strncpy(name, EclGetButton("nametext 0 0")->Caption, 0x40);
	strncpy(password, EclGetButton("passwordtext 0 0")->Caption, 0x21);
	strncpy(password2, EclGetButton("passwordtext2 0 0")->Caption, 0x21);

	if (strcmp(name, "Fill this in") == 0)
	{
		create_msgbox("Error", "You must first enter a username", nullptr);
		return;
	}

	if (strchr(name, ':') != nullptr || strchr(name, '/') != nullptr || strchr(name, '\\') != nullptr || strchr(name, '?') != nullptr ||
		strchr(name, '.') != nullptr || strchr(name, ',') != nullptr || strchr(name, '\"') != nullptr || strchr(name, '<') != nullptr ||
		strchr(name, '>') != nullptr || strchr(name, '|') != nullptr || strchr(name, '*') != nullptr)
	{
		create_msgbox("Error", "Usernames cannot contain\nany of these characters:\n : / \\ ? . , \" < > | * ", nullptr);
		return;
	}

	if (strcmp(name, "admin") == 0 || strcmp(name, "readwrite") == 0 || strcmp(name, "readonly") == 0)
	{
		create_msgbox("Error", "Usernames cannot be\nadmin, readwrite or readonly\n", nullptr);
		return;
	}

	if (password[0] == 0)
	{
		create_msgbox("Error", "You must first enter a password", nullptr);
		return;
	}

	if (strcmp(password, password2) != 0)
	{
		create_msgbox("Error", "The two passwords must be identical", nullptr);
		return;
	}

	Computer::GenerateAccessCode(name, password, accessCode, 0x80);

	game->GetWorld().GetPlayer().SetHandle(name);

	puts("TODO: implement Script33");

	game->GetInterface().GetRemoteInterface().RunScreen(4, nullptr);
}

static void Script34()
{
	if (EclGetButton("start_localhost") != nullptr)
	{
		if (EclGetButton("start_link") == nullptr)
			return;

		EclRemoveButton("start_link");

		GciTimerFunc(600, ScriptLibrary::RunScript, 35);

		return;
	}

	EclRegisterButton(230, 200, 64, 78, " ", " ", "start_localhost");
	button_assignbitmap("start_localhost", LOCALHOST_IMAGE);
	EclRegisterButton(330, 220, 32, 32, " ", " ", "start_key");
	button_assignbitmap("start_key", "analyser/nonsecure.tif");

	const auto startKey = EclGetButton("start_key");
	UplinkAssert(startKey != nullptr);

	EclRegisterButton(startKey->X - 43, startKey->Y + (startKey->Height / 2) - 1, 42, 4, " ", " ", "start_keylink");
	EclRegisterButtonCallbacks("start_keylink", DrawConnection, nullptr, nullptr, nullptr);
	EclRegisterButton(450, 340, 32, 32, " ", " ", "start_lock");
	button_assignbitmap("start_lock", "analyser/secure.tif");

	const auto startLock = EclGetButton("start_lock");
	UplinkAssert(startLock != nullptr);

	EclRegisterButton(startLock->X + startLock->Width, startLock->Y + (startLock->Height / 2) - 1, 31, 4, " ", " ", "start_locklink");
	EclRegisterButtonCallbacks("start_locklink", DrawConnection, 0, 0, nullptr);

	const auto startLinkSize = startLock->X - startKey->X - 16;

	EclRegisterButton(startKey->X + (startKey->Width / 2) + 8, startKey->Y + (startKey->Height / 2) + 8, startLinkSize, startLinkSize, " ", " ",
					  "start_link");
	EclRegisterButtonCallbacks("start_link", DrawConnection, nullptr, nullptr, nullptr);
	EclRegisterCaptionChange("connecting 0 0", "Disconnecting from Uplink Public access server...", 700, nullptr);
	GciTimerFunc(800, ScriptLibrary::RunScript, 34);
}

static void Script35()
{
	if (EclGetButton("start_target") == nullptr)
	{
		const auto dialler = new PhoneDialler();
		dialler->DialNumber(400, 170, "127.0.0.1", PhoneDiallerNextScene::Script93, nullptr);
		return;
	}

	if (EclGetButton("start_target")->X < 640)
	{
		const auto startLock = EclGetButton("start_lock");
		UplinkAssert(startLock != nullptr);

		EclRegisterMovement("start_target", 690, 320, 500, nullptr);
		EclRegisterMovement("start_lock", 650, 340, 500, nullptr);
		EclRegisterMovement("start_locklink", startLock->X + startLock->Width + 200, startLock->Y + (startLock->Height / 2) - 1, 500, nullptr);
		SgPlaySound(RsArchiveFileOpen("sounds/newmailreverse.wav"), "sounds/newmailreverse.wav");
		EclRegisterCaptionChange("connecting 0 0", "Establishing connection to your GATEWAY computer...", 600, nullptr);
	}
	else
	{
		EclRemoveButton("start_target");
		EclRegisterButton(706, 320, 64, 80, " ", " ", "start_target2");
		button_assignbitmap("start_target2", "start/gateway.tif");
		EclButtonSendToBack("start_target2");

		const auto startLock = EclGetButton("start_lock");
		UplinkAssert(startLock != nullptr);

		EclRegisterMovement("start_target2", 506, 320, 500, nullptr);
		EclRegisterMovement("start_lock", 450, 340, 500, nullptr);
		EclRegisterMovement("start_locklink", startLock->Width + 450, (startLock->Height / 2) + 340 - 1, 500, nullptr);
		SgPlaySound(RsArchiveFileOpen("sounds/newmail.wav"), "sounds/newmail.wav");
	}

	GciTimerFunc(1000, ScriptLibrary::RunScript, 35);
}

static void Script36()
{
	EclRemoveButton("start_localhost");
	EclRemoveButton("start_key");
	EclRemoveButton("start_keylink");
	EclRemoveButton("start_target2");
	EclRemoveButton("start_lock");
	EclRemoveButton("start_locklink");
	EclRemoveButton("start_link");
	game->GetWorld().GetPlayer().GetConnection().Disconnect();
	game->GetWorld().GetPlayer().GetConnection().Reset();
	game->GetInterface().GetRemoteInterface().RunNewLocation();
	game->GetInterface().GetRemoteInterface().RunScreen(5, nullptr);
	GciTimerFunc(100, ScriptLibrary::RunScript, 40);
}

static void Script40()
{
	if (EclGetButton("downloadingUOS") == nullptr)
	{
		EclRegisterButton(180, 180, 300, 20, "Downloading UPLINK Operating System...", "", "downloadingUOS_title");
		EclRegisterButtonCallbacks("downloadingUOS_title", textbutton_draw, 0, 0, nullptr);
		EclRegisterButton(190, 210, 0, 15, "", "Shows the progress of the download", "downloadingUOS");
		EclRegisterResize("downloadingUOS", 200, 15, 5000, nullptr);
		EclRegisterButton(150, 210, 40, 15, "", "0 %", "downloadingUOS_prog");
		EclRegisterButtonCallbacks("downloadingUOS_prog", textbutton_draw, 0, 0, nullptr);
		EclRegisterButton(190, 230, 300, 15, " ", " ", "downloadingUOS_current");
		EclRegisterButtonCallbacks("downloadingUOS_current", textbutton_draw, 0, 0, nullptr);
		GciTimerFunc(100, ScriptLibrary::RunScript, 40);
		return;
	}

	const auto downloadingButton = EclGetButton("downloadingUOS");
	UplinkAssert(downloadingButton != nullptr);

	const auto prog = EclGetButton("downloadingUOS_prog");
	UplinkAssert(prog != nullptr);

	const auto current = EclGetButton("downloadingUOS_current");
	UplinkAssert(current != nullptr);

	const int progress = downloadingButton->Width * 100.0 / 200.0;

	char progressString[0x10];
	UplinkSnprintf(progressString, 0x10, "%d %%", progress);

	prog->SetCaption(progressString);

	if (progress <= 4)
	{
		current->SetCaption("Downloading Core services");
		GciTimerFunc(100, ScriptLibrary::RunScript, 40);
		return;
	}

	if (progress <= 9)
	{
		current->SetCaption("Downloading Kernel");
		GciTimerFunc(100, ScriptLibrary::RunScript, 40);
		return;
	}

	if (progress <= 14)
	{
		current->SetCaption("Downloading Loader package");
		GciTimerFunc(100, ScriptLibrary::RunScript, 40);
		return;
	}

	if (progress <= 19)
	{
		current->SetCaption("Downloading Gateway package");
		GciTimerFunc(100, ScriptLibrary::RunScript, 40);
		return;
	}

	if (progress <= 24)
	{
		current->SetCaption("Downloading Memory package");
		GciTimerFunc(100, ScriptLibrary::RunScript, 40);
		return;
	}

	if (progress <= 29)
	{
		current->SetCaption("Downloading Status package");
		GciTimerFunc(100, ScriptLibrary::RunScript, 40);
		return;
	}

	if (progress <= 34)
	{
		current->SetCaption("Downloading Finance package");
		GciTimerFunc(100, ScriptLibrary::RunScript, 40);
		return;
	}

	if (progress <= 39)
	{
		current->SetCaption("Downloading Mail package");
		GciTimerFunc(100, ScriptLibrary::RunScript, 40);
		return;
	}

	if (progress <= 44)
	{
		current->SetCaption("Downloading Mission package");
		GciTimerFunc(100, ScriptLibrary::RunScript, 40);
		return;
	}

	if (progress <= 49)
	{
		current->SetCaption("Downloading Clock package");
		GciTimerFunc(100, ScriptLibrary::RunScript, 40);
		return;
	}

	if (progress <= 54)
	{
		current->SetCaption("Downloading Multitasking package");
		GciTimerFunc(100, ScriptLibrary::RunScript, 40);
		return;
	}

	if (progress <= 59)
	{
		current->SetCaption("Downloading Map package");
		GciTimerFunc(100, ScriptLibrary::RunScript, 40);
		return;
	}

	if (progress <= 74)
	{
		current->SetCaption("Downloading Global communications package");
		GciTimerFunc(100, ScriptLibrary::RunScript, 40);
		return;
	}

	if (progress <= 79)
	{
		current->SetCaption("Downloading Security package");
		GciTimerFunc(100, ScriptLibrary::RunScript, 40);
		return;
	}

	if (progress <= 84)
	{
		current->SetCaption("Downloading File utilities");
		GciTimerFunc(100, ScriptLibrary::RunScript, 40);
		return;
	}

	if (progress <= 89)
	{
		current->SetCaption("Downloading Tutorial daemon");
		GciTimerFunc(100, ScriptLibrary::RunScript, 40);
		return;
	}

	if (progress <= 94)
	{
		current->SetCaption("Downloading Remote renderer package");
		GciTimerFunc(100, ScriptLibrary::RunScript, 40);
		return;
	}

	if (progress <= 99)
	{
		current->SetCaption("Downloading Site connect package");
		GciTimerFunc(100, ScriptLibrary::RunScript, 40);
		return;
	}

	GciTimerFunc(500, ScriptLibrary::RunScript, 41);
}

static void Script41()
{
	if (EclGetButton("syscheckUOS_13") != nullptr)
	{
		GciTimerFunc(250, ScriptLibrary::RunScript, 42);
		return;
	}

	for (auto i = 0; i < 14; i++)
	{
		char var_48[0x20];
		UplinkSnprintf(var_48, 0x20, "syscheckUOS_%d", i);

		if (EclGetButton(var_48) != nullptr)
			continue;

		auto caption = "Checking gateway HARDWARE : Trinity Ax086 2GQs cpu...";
		if (i == 0)
		{
			EclRemoveButton("downloadingUOS_title");
			EclRemoveButton("downloadingUOS_prog");
			EclRemoveButton("downloadingUOS_current");
			EclRemoveButton("downloadingUOS");
		}
		else
		{
			char var_68[0x20];
			UplinkSnprintf(var_68, 0x20, "syscheckUOS_OK_%d", i - 1);

			EclRegisterButton(450, 110 + (20 * i), 30, 15, "[OK]", "", var_68);
			EclRegisterButtonCallbacks(var_68, textbutton_draw, nullptr, nullptr, nullptr);
			switch (i)
			{
				case 1:
					caption = "Checking gateway HARDWARE : K256 512KQds modem...";
					break;
				case 2:
					caption = "Checking gateway HARDWARE : 24 GQds DR-RAM memory...";
					break;
				case 3:
					caption = "Verifying integrity of Uplink Operating System...";
					break;
				case 4:
					caption = "Verifying security of Uplink Operating System...";
					break;
				case 5:
					caption = "Installing UPLINK Operating system files...";
					break;
				case 6:
					caption = "Initialising TSR programs in memory...";
					break;
				case 7:
					caption = "Sending RUN signal to core Uplink Kernel...";
					break;
				case 8:
					caption = "Checking Kernel Initilisation...";
					break;
				case 9:
					caption = "Establishing Network communications protocols...";
					break;
				case 10:
					caption = "Installing File-Copier...";
					break;
				case 11:
					caption = "Installing File-Deleter...";
					break;
				case 12:
					caption = "Installing Task-Manager you can read fast...";
					break;
				case 13:
					caption = "Running Graphical User Interface...";
					break;
			}
		}

		EclRegisterButton(20, 130 + (20 * i), 400, 15, caption, "", var_48);
		EclRegisterButtonCallbacks(var_48, textbutton_draw, nullptr, nullptr, nullptr);
		SgPlaySound(RsArchiveFileOpen("sounds/done.wav"), "sounds/done.wav");
		GciTimerFunc(250, ScriptLibrary::RunScript, 41);
		return;
	}
}

static void Script42()
{
	if (EclGetButton("syscheckUOS_0") != nullptr)
	{
		for (auto i = 0; i < 14; i++)
		{
			char buttonName[0x20];

			UplinkSnprintf(buttonName, 0x20, "syscheckUOS_%d", i);
			EclRemoveButton(buttonName);

			UplinkSnprintf(buttonName, 0x20, "syscheckUOS_OK_%d", i);
			EclRemoveButton(buttonName);
		}
	}

	if (!game->GetInterface().GetLocalInterface().IsVisible())
	{
		game->GetInterface().GetLocalInterface().Reset();
		game->GetInterface().GetLocalInterface().Create();
	}

	app->GetOptions()->SetOptionValue("game_firsttime", 0);
	game->GetInterface().GetRemoteInterface().RunScreen(6, nullptr);
}

static void Script91()
{
	app->LoadGame();
}

static void Script92()
{
	const auto key = EclGetButton("start_key");
	const auto lock = EclGetButton("start_lock");
	UplinkAssert(key != nullptr);
	UplinkAssert(lock != nullptr);
	const auto size = lock->X - key->X - 16;
	EclRegisterButton(key->X + (key->Width / 2) + 8, key->Y + 8 + (key->Height / 2), size, size, " ", " ", "start_link");
	EclRegisterButtonCallbacks("start_link", DrawConnection, 0, 0, nullptr);
	EclRegisterCaptionChange("firsttimeloading_text", "Awaiting connection acknowledgment from UPLINK...", 800, nullptr);
	SgPlaySound(RsArchiveFileOpen("sounds/ringout.wav"), "sounds/ringout.wav");
	GciTimerFunc(900, ScriptLibrary::RunScript, 32);
}

static void Script93()
{
	struct Button* key = EclGetButton("start_key");
	struct Button* lock = EclGetButton("start_lock");
	UplinkAssert(key != nullptr);
	UplinkAssert(lock != nullptr);
	int32_t size = lock->X - key->X - 16;
	EclRegisterButton(key->X + (key->Width / 2) + 8, key->Y + (key->Height / 2) + 8, size, size, " ", " ", "start_link");
	EclRegisterButtonCallbacks("start_link", DrawConnection, 0, 0, nullptr);
	EclRegisterCaptionChange("connecting 0 0", "Awaiting connection acknowledgment from GATEWAY...", 0x2bc, nullptr);
	GciTimerFunc(3500, ScriptLibrary::RunScript, 36);
}

void ScriptLibrary::RunScript(int index)
{
	switch (index)
	{
		case 30:
			Script30();
			return;
		case 31:
			Script31();
			return;
		case 32:
			Script32();
			return;
		case 33:
			Script33();
			return;
		case 34:
			Script34();
			return;
		case 35:
			Script35();
			return;
		case 36:
			Script36();
			return;
		case 40:
			Script40();
			return;
		case 41:
			Script41();
			return;
		case 42:
			Script42();
			return;
		case 91:
			Script91();
			return;
		case 92:
			Script92();
			return;
		case 93:
			Script93();
			return;
		default:
			printf("WARNING: Unknown script index %d\n", index);
			UplinkAbort("TODO: Script not implemented!!");
			break;
	}
}
