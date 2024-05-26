#include <Eclipse.hpp>
#include <GL/gl.h>
#include <Gci.hpp>
#include <Globals.hpp>
#include <Opengl.hpp>
#include <RedShirt.hpp>
#include <ScriptLibrary.hpp>
#include <Sg.hpp>

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
		button_assignbitmap("start_localhost", "start/localhost_linux.tif");
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
		default:
			printf("WARNING: Unknown script index %d\n", index);
			UplinkAbort("TODO: Script not implemented!!");
			break;
	}
}
