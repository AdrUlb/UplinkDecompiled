#include <Opengl.hpp>

#include <App.hpp>
#include <Eclipse.hpp>
#include <GL/gl.h>
#include <Gci.hpp>
#include <Globals.hpp>
#include <IRCInterface.hpp>
#include <RedShirt.hpp>
#include <ScrollBox.hpp>
#include <Sg.hpp>

static int lastidleupdate = 0;
bool data_7b295f = false;
static int curserflash = 0;
int tooltipanimindex = -1;

static int mouseX = 0;
static int mouseY = 0;

static char currentbuttonname[0x200];

static LList<char*>* wordwraptext(const char* text, int width)
{
	if (text == nullptr)
		return nullptr;

	const auto ret = new LList<char*>();

	if (text[0] == 0)
	{
		const auto str = new char[2];
		str[0] = 0;

		ret->PutData(str);
		return ret;
	}

	const auto avgCharWidth = GciTextWidth("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ") / 52.0f;
	auto charsPerLine = static_cast<int>(width / avgCharWidth);
	if (charsPerLine == 0)
		charsPerLine = 1;

	const auto textLength = strlen(text);
	auto line = new char[textLength + 3];
	UplinkSnprintf(line, textLength + 2, "%s\n", text);

	ret->PutData(line);

	while (true)
	{
		// Find the next newline
		char* newlineIndex = strchr(line, '\n');
		if (newlineIndex == 0)
			break;

		// If the next newline is within the maximum width allowed
		if (newlineIndex <= line + charsPerLine)
		{
			// Replace newline with null terminator
			*newlineIndex = 0;

			// Next line begins right after newline
			line = newlineIndex + 1;

			// Add line to list
			ret->PutData(line);

			continue;
		}

		// Save the last character, replace it with a null terminator
		const auto lastChar = line[charsPerLine - 1];
		line[charsPerLine - 1] = 0;

		// Find the last space in the current line, if there is none, advance the line position, add it to the list and continue
		auto spaceIndex = strrchr(line, ' ');
		if (spaceIndex == 0)
		{
			line += charsPerLine;
			ret->PutData(line);
			continue;
		}

		// If a space was found, replace it with a null terminator, put back the character that was replaced, and continue
		*spaceIndex = 0;
		line[charsPerLine - 1] = lastChar;
		line = spaceIndex + 1;
		ret->PutData(line);
	}

	return ret;
}

static void text_draw(Button* button, bool highlighted, bool clicked)
{
	(void)clicked;

	UplinkAssert(button != nullptr);

	const auto screenheight = app->GetOptions()->GetOptionValue("graphics_screenheight");
	glScissor(button->X, screenheight - button->Y - button->Height, button->Width, button->Height);
	glEnable(GL_SCISSOR_TEST);
	SetColour("DefaultText");

	LList<char*>* wrappedText;
	if (highlighted && EclIsButtonEditable(button->Name))
	{
		if (time(nullptr) < curserflash - 1)
		{
			wrappedText = wordwraptext(button->Caption, button->Width);
		}
		else
		{
			const auto captionLength = strlen(button->Caption) + 2;
			char* rax_13 = new char[captionLength];
			UplinkSnprintf(rax_13, captionLength, "%s_", button->Caption);

			wrappedText = wordwraptext(rax_13, button->Width);
			delete[] rax_13;

			if (time(nullptr) >= curserflash)
				curserflash = (time(nullptr) + 2);
		}
	}
	else
		wrappedText = wordwraptext(button->Caption, button->Width);

	if (wrappedText == nullptr)
	{
		glDisable(GL_SCISSOR_TEST);
		return;
	}

	auto yOffset = 0;
	for (auto i = 0; i < wrappedText->Size(); i++)
	{
		GciDrawText(button->X + 10, button->Y + 10 + yOffset, wrappedText->GetData(i));
		yOffset += 15;
	}

	if (wrappedText->ValidIndex(0) && wrappedText->GetData(0) != nullptr)
		delete[] wrappedText->GetData(0);

	delete wrappedText;

	glDisable(GL_SCISSOR_TEST);
}

static void tooltip_callback()
{
	tooltipanimindex = -1;
}

static void tooltip_update(const char* text)
{
	auto tooltipButton = EclGetButton("tooltip");

	if (tooltipButton == nullptr)
	{
		app->GetOptions()->GetOptionValue("graphics_screenwidth");
		EclRegisterButton(0, app->GetOptions()->GetOptionValue("graphics_screenheight") - 15, 500, 15, "", "tooltip");
		EclRegisterButtonCallbacks("tooltip", textbutton_draw, 0, 0, nullptr);
		EclButtonSendToBack("tooltip");
		tooltipButton = EclGetButton("tooltip");
	}

	if (strcmp(tooltipButton->Tooltip, text) == 0)
		return;

	tooltipButton->SetTooltip(text);

	if (tooltipanimindex != -1)
	{
		EclRemoveAnimation(tooltipanimindex);
		tooltipanimindex = -1;
	}

	if (strcmp(text, " ") == 0)
	{
		tooltipButton->SetCaption(" ");
		currentbuttonname[0] = 0;
	}
	else
		tooltipanimindex = EclRegisterCaptionChange("tooltip", text, tooltip_callback);
}

static void superhighlight_draw(Button* button)
{
	(void)button;
	UplinkAbort("TODO: implement superhighlight_draw()");
}

void button_draw(Button* button, bool highlighted, bool clicked)
{
	UplinkAssert(button != 0);
	auto screenHeight = app->GetOptions()->GetOptionValue("graphics_screenheight");

	glScissor(button->X, screenHeight - button->Y - button->Height, button->Width, button->Height);
	glEnable(GL_SCISSOR_TEST);

	glBegin(GL_QUADS);
	if (clicked != 0)
	{
		SetColour("ButtonClickedA");
		glVertex2i(button->X, button->Y + button->Height);
		SetColour("ButtonClickedB");
		glVertex2i(button->X, button->Y);
		SetColour("ButtonClickedA");
		glVertex2i(button->X + button->Width, button->Y);
		SetColour("ButtonClickedB");
		glVertex2i(button->X + button->Width, button->Y + button->Height);
	}
	else if (highlighted == 0)
	{
		SetColour("ButtonNormalA");
		glVertex2i(button->X, button->Y + button->Height);
		SetColour("ButtonNormalB");
		glVertex2i(button->X, button->Y);
		SetColour("ButtonNormalA");
		glVertex2i(button->X + button->Width, button->Y);
		SetColour("ButtonNormalB");
		glVertex2i(button->X + button->Width, button->Y + button->Height);
	}
	else
	{
		SetColour("ButtonHighlightedA");
		glVertex2i(button->X, button->Y + button->Height);
		SetColour("ButtonHighlightedB");
		glVertex2i(button->X, button->Y);
		SetColour("ButtonHighlightedA");
		glVertex2i(button->X + button->Width, button->Y);
		SetColour("ButtonHighlightedB");
		glVertex2i(button->X + button->Width, button->Y + button->Height);
	}
	glEnd();

	auto textWidth = GciTextWidth(button->Caption);
	SetColour("DefaultText");
	const auto x = (button->Width / 2) + button->X - (textWidth / 2);
	const auto y = button->Y + 2 + (button->Height / 2);
	GciDrawText(x, y, button->Caption);
	glDisable(GL_SCISSOR_TEST);
}

void clear_draw(int x, int y, int width, int height)
{
	SetColour("Background");
	const auto right = x + width;
	const auto bottom = y + height;

	glBegin(GL_QUADS);
	glVertex2i(x, y);
	glVertex2i(right, y);
	glVertex2i(right, bottom);
	glVertex2i(x, bottom);
	glEnd();
}

void button_click(Button* button)
{
	UplinkAssert(button != nullptr);
	EclClickButton(button->Name);
	SgPlaySound(RsArchiveFileOpen("sounds/mouseclick.wav"), "sounds/mouseclick.wav");
}

void button_highlight(Button* button)
{
	UplinkAssert(button != nullptr);

	EclHighlightButton(button->Name);
	tooltip_update(button->Tooltip);

	if (strcmp(button->Name, currentbuttonname) == 0)
		return;

	UplinkStrncpy(currentbuttonname, button->Name, 0x200);
}

void SetColour(const char* name)
{
	Options* options;
	ColourOption* colour;

	if (app == nullptr || (options = app->GetOptions()) == nullptr || (colour = options->GetColour(name)) == nullptr)
	{
		printf("SetColour WARNING : Failed to find colour %s\n", name);
		glColor3f(0.0f, 0.0f, 0.0f);
		return;
	}

	glColor3f(colour->red, colour->green, colour->blue);
}

void border_draw(Button* button)
{
	glBegin(GL_LINE_LOOP);
	glVertex2i(button->X, button->Y);
	glVertex2i(button->X + button->Width - 1, button->Y);
	glVertex2i(button->X + button->Width - 1, button->Y + button->Height - 1);
	glVertex2i(button->X, button->Y + button->Height - 1);
	glEnd();
}

void imagebutton_draw(Button* button, bool highlighted, bool clicked)
{
	const auto screenHeight = app->GetOptions()->GetOptionValue("graphics_screenheight");
	glScissor(button->X, screenHeight - button->Y - button->Height, button->Width, button->Height);
	glEnable(GL_SCISSOR_TEST);

	Image* image;
	if (clicked && button->ImageClicked != nullptr)
	{
		image = button->ImageClicked;
	}
	else if (highlighted && button->ImageHighlighted != nullptr)
	{
		image = button->ImageHighlighted;
	}
	else
	{
		image = button->ImageNormal;
	}

	image->Draw(button->X, button->Y);

	glDisable(GL_SCISSOR_TEST);
}

void textbutton_draw(Button* button, bool highlighted, bool clicked)
{
	UplinkAssert(button != nullptr);
	auto screenHeight = app->GetOptions()->GetOptionValue("graphics_screenheight");
	glScissor(button->X, screenHeight - button->Y - button->Height, button->Width, button->Height);
	glEnable(GL_SCISSOR_TEST);
	clear_draw(button->X, button->Y, button->Width, button->Height);
	text_draw(button, highlighted, clicked);

	if (highlighted || clicked)
	{
		SetColour("TextBorder");
		border_draw(button);
	}

	glDisable(GL_SCISSOR_TEST);
}

static void display()
{
	if (app->Closed() || !GciAppVisible())
		return;

	glPushMatrix();
	glLoadIdentity();

	glMatrixMode(GL_PROJECTION);

	glPushMatrix();
	glLoadIdentity();

	glPushAttrib(GL_ALL_ATTRIB_BITS);

	const auto screenWidth = app->GetOptions()->GetOptionValue("graphics_screenwidth");
	const auto screenHeight = app->GetOptions()->GetOptionValue("graphics_screenheight");
	glOrtho(0.0, screenWidth, screenHeight, 0.0, -1.0, 1.0);

	glTranslatef(0.375f, 0.375f, 0.0f);
	EclClearRectangle(0, 0, screenWidth, screenHeight);
	EclDrawAllButtons();

	glPopAttrib();

	glPopMatrix();

	glMatrixMode(GL_MODELVIEW);

	glPopMatrix();

	GciSwapBuffers();
	glFinish();
}

static void mousedraw(Button* button, bool highlighted, bool clicked)
{
	(void)highlighted;
	(void)clicked;
	UplinkAssert(button != nullptr);

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glBegin(GL_LINES);
	glLineWidth(1.0f);
	glVertex2i(button->X, button->Y);
	glVertex2i(button->X + button->Width - 1, button->Y + button->Height - 1);
	glVertex2i(button->X, button->Y);
	glVertex2i(button->X + (button->Width / 1.5), button->Y);
	glVertex2i(button->X + (button->Width / 1.5), button->Y);
	glVertex2i(button->X, button->Y + (button->Height / 1.5));
	glEnd();
	glLineWidth(1.0f);
}

static void mouse(GciMouseButton button, GciMouseEvent event, int x, int y)
{
	if (app->Closed())
		return;

	if (button == GciMouseButton::Left && event == GciMouseEvent::Down)
	{
		const auto buttonName = EclGetButtonAtCoord(x, y);
		if (buttonName != nullptr)
		{
			const auto button = EclGetButton(buttonName);
			if (button != nullptr)
				button->MouseDown();
		}

		return;
	}

	if (button == GciMouseButton::Left && event == GciMouseEvent::Up)
	{
		EclUnClickButton();
		ScrollBox::UnGrabScrollBar();
		const auto buttonName = EclGetButtonAtCoord(x, y);
		if (buttonName != 0)
		{
			const auto button = EclGetButton(buttonName);
			if (button != nullptr)
				button->MouseUp();
		}
		else if (game->IsRunning() && SWInterface::IsVisibleSoftwareMenu())
			SWInterface::ToggleSoftwareMenu();

		return;
	}

	if (button == GciMouseButton::Right && event == GciMouseEvent::Up)
	{
		if (game->IsRunning())
			game->GetInterface()->GetTaskManager()->SetTargetProgram(-1);
	}
}

static void mousemove(int x, int y)
{
	if (app->Closed())
		return;

	mouseX = x;
	mouseY = y;

	if (ScrollBox::IsGrabInProgress())
		ScrollBox::UpdateGrabScroll();

	if (app->GetOptions()->IsOptionEqualTo("graphics_softwaremouse", 1))
	{
		if (EclGetButton("mouse") == nullptr)
		{
			EclRegisterButton(0, 0, 8, 8, "", "mouse");
			EclRegisterButtonCallbacks("mouse", mousedraw, 0, 0, nullptr);
		}

		EclButtonBringToFront("mouse");

		const auto mouse = EclGetButton("mouse");

		UplinkAssert(mouse != nullptr);

		mouse->X = x + 1;
		mouse->Y = y + 1;
	}
}

static void passivemouse(int x, int y)
{
	if (app->Closed())
		return;
	mouseX = x;
	mouseY = y;

	if (app->GetOptions()->IsOptionEqualTo("graphics_softwaremouse", 1))
	{
		if (EclGetButton("mouse") == 0)
		{
			EclRegisterButton(0, 0, 8, 8, "", "mouse");
			EclRegisterButtonCallbacks("mouse", mousedraw, 0, 0, nullptr);
		}
		EclButtonBringToFront("mouse");
		const auto mouse = EclGetButton("mouse");
		UplinkAssert(mouse != nullptr);
		mouse->X = x + 1;
		mouse->Y = y + 1;
	}

	const auto buttonName = EclGetButtonAtCoord(x, y);
	if (buttonName == nullptr)
	{
		EclUnHighlightButton();
		tooltip_update(" ");
	}
	else
	{
		const auto button = EclGetButton(buttonName);
		if (button != nullptr)
			return button->MouseMove();
	}
}

static void keyboard(char keychar)
{
	if (app->Closed())
		return;

	// Return key
	if (keychar == '\r')
	{
		if (!game->IsRunning())
		{
			if (app->GetMainMenu()->InScreen() == MainMenuScreenCode::Unknown || !app->GetMainMenu()->GetMenuScreen()->ReturnKeyPressed())
				goto highlighted;

			return;
		}

		if (game->GetInterface()->GetLocalInterface()->InScreen() == 0)
		{
			if (!game->GetInterface()->GetRemoteInterface()->GetInterfaceScreen()->ReturnKeyPressed())
				goto highlighted;

			return;
		}

		if (game->GetInterface()->GetLocalInterface()->GetInterfaceScreen()->ScreenID() == 16)
		{
			if (!IRCInterface::ReturnKeyPressed())
				goto highlighted;

			return;
		}

		if (!game->GetInterface()->GetRemoteInterface()->GetInterfaceScreen()->ReturnKeyPressed())
			goto highlighted;

		return;
	}

	// Escape key
	if (keychar == 27)
	{
		const auto highlightedButtonName = EclGetHighlightedButton();
		if (EclIsButtonEditable(highlightedButtonName))
		{
			textbutton_keypress(EclGetButton(highlightedButtonName), 27);
			return;
		}

		if (game->IsRunning())
			game->GetInterface()->GetRemoteInterface()->GetInterfaceScreen()->EscapeKeyPressed();

		return;
	}

	// Tab key
	if (keychar == 9)
	{
		EclHighlightNextEditableButton();
		return;
	}

	// Grave accent key
	if (keychar == '`')
	{
		char s[0x100];
		UplinkSnprintf(s, 0x100, "%sscreenshot.bmp", &app->usersPath);
		GciSaveScreenshot(s);
		return;
	}

	// Any other key
highlighted:
	const auto highlightedButtonName = EclGetHighlightedButton();
	if (EclIsButtonEditable(highlightedButtonName))
		textbutton_keypress(EclGetButton(highlightedButtonName), keychar);
}

static void specialkeyboard(int keycode)
{
	(void)keycode;
	puts("TODO: implement specialkeyboard()");
}

static void idle()
{
	if (app->Closed())
		return;
	EclGetAccurateTime();
	EclUpdateAllAnimations();
	SgUpdate();
	app->Update();
	lastidleupdate = EclGetAccurateTime();
}

static void resize(int width, int height)
{
	(void)width;
	(void)height;
	UplinkAbort("TODO: implement resize()");
}

static void setcallbacks()
{
	GciDisplayFunc(display);
	GciMouseFunc(mouse);
	GciMotionFunc(mousemove);
	GciPassiveMotionFunc(passivemouse);
	GciKeyboardFunc(keyboard);
	GciSpecialFunc(specialkeyboard);
	GciIdleFunc(idle);
	GciReshapeFunc(resize);
}

void opengl_initialise()
{
	const auto debug = app->GetOptions()->IsOptionEqualTo("game_debugstart", 1);

	auto width = app->GetOptions()->GetOptionValue("graphics_screenwidth");
	auto height = app->GetOptions()->GetOptionValue("graphics_screenheight");
	const auto depth = app->GetOptions()->GetOptionValue("graphics_screendepth");
	const auto fullscreen = app->GetOptions()->IsOptionEqualTo("graphics_fullscreen", 1);
	const auto safemode = app->GetOptions()->IsOptionEqualTo("graphics_safemode", 1);

	GciInitFlags flags{.UnknownFlag0 = true, .UnknownFlag1 = true, .Fullscreen = fullscreen && !safemode, .Debug = debug};

	if (const auto errorMessage = GciInitGraphicsLibrary(flags); errorMessage != nullptr)
		UplinkAbort(errorMessage);

	const auto screenMode = GciGetClosestScreenMode(width, height);
	if (screenMode[0] != width || screenMode[1] != height)
	{
		width = screenMode[0];
		height = screenMode[1];
		app->GetOptions()->SetOptionValue("graphics_screenwidth", width);
		app->GetOptions()->SetOptionValue("graphics_screenheight", height);
	}
	delete[] screenMode;

	if (const auto errorMessage = GciInitGraphics("uplink", flags, width, height, depth); errorMessage != nullptr)
		UplinkAbort(errorMessage);

	const auto actualScreenMode = GciGetCurrentScreenMode();
	if (actualScreenMode[0] != width || actualScreenMode[1] != height)
	{
		printf("We actually ended up with %dx%d\n", actualScreenMode[0], actualScreenMode[1]);
		app->GetOptions()->SetOptionValue("graphics_screenwidth", actualScreenMode[0]);
		app->GetOptions()->SetOptionValue("graphics_screenheight", actualScreenMode[1]);
	}

	delete[] actualScreenMode;

	if (debug)
		puts("Initialising OpenGL...");

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glDisable(GL_DEPTH_TEST);
	glMatrixMode(GL_MODELVIEW);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glDisable(GL_ALPHA_TEST);
	glDisable(GL_FOG);
	glDisable(GL_LIGHTING);
	glDisable(GL_INDEX_LOGIC_OP);
	glDisable(GL_STENCIL_TEST);
	glDisable(GL_TEXTURE_1D);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);

	glPixelTransferi(GL_MAP_COLOR, 0);
	glPixelTransferi(GL_RED_SCALE, 1);
	glPixelTransferi(GL_RED_BIAS, 0);
	glPixelTransferi(GL_GREEN_SCALE, 1);
	glPixelTransferi(GL_GREEN_BIAS, 0);
	glPixelTransferi(GL_BLUE_SCALE, 1);
	glPixelTransferi(GL_BLUE_BIAS, 0);
	glPixelTransferi(GL_ALPHA_SCALE, 1);
	glPixelTransferi(GL_ALPHA_BIAS, 0);

	GLuint tex;
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, (tex));
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);

	EclReset();
	EclRegisterClearDrawFunction(clear_draw);
	EclRegisterDefaultButtonCallbacks(button_draw, nullptr, button_click, button_highlight);
	EclRegisterSuperHighlightFunction(3, superhighlight_draw);
	const auto buttonAnimationsOption = app->GetOptions()->GetOption("graphics_buttonanimations");

	if (buttonAnimationsOption && !buttonAnimationsOption->GetValue())
		EclDisableAnimations();

	const auto value = app->GetOptions()->GetOption("graphics_fasterbuttonanimations");

	if (value != nullptr && app->GetOptions()->GetOption("graphics_fasterbuttonanimations")->GetValue())
		EclEnableFasterAnimations(2.0);

	if (debug)
	{
		puts("Finished initialising OpenGL.");
		printf("Now registering callback functions...");
	}

	setcallbacks();

	if (debug)
		printf("done\n ");
}

void opengl_run()
{
	GciMainLoop();
}

void opengl_close()
{
	GciRestoreScreenSize();
}

void button_assignbitmap(const char* buttonName, const char* imageName)
{
	const auto button = EclGetButton(buttonName);
	UplinkAssert(button != nullptr);

	const char* filePath = app->GetOptions()->ThemeFilename(imageName);

	const auto image = new Image();
	image->LoadTIF(RsArchiveFileOpen(filePath));
	image->SetAlpha(0.85f);

	delete[] filePath;

	button->SetStandardImage(image);
	button->RegisterDrawFunction(imagebutton_draw);
}

void button_assignbitmaps(const char* buttonName, const char* normalFile, const char* highlightedFile, const char* clickedFile)
{
	const auto button = EclGetButton(buttonName);
	UplinkAssert(button != nullptr);

	const auto normalPath = app->GetOptions()->ThemeFilename(normalFile);
	const auto imageNormal = new Image();
	imageNormal->LoadTIF(RsArchiveFileOpen(normalPath));
	imageNormal->SetAlpha(0.85f);
	if (normalPath != 0)
		delete[] normalPath;

	const auto highlightedPath = app->GetOptions()->ThemeFilename(highlightedFile);
	const auto imageHighlighted = new Image();
	imageHighlighted->LoadTIF(RsArchiveFileOpen(highlightedPath));
	imageHighlighted->SetAlpha(0.85f);
	if (highlightedPath != 0)
		delete[] highlightedPath;

	const auto clickedPath = app->GetOptions()->ThemeFilename(clickedFile);
	const auto imageClicked = new Image();
	imageClicked->LoadTIF(RsArchiveFileOpen(clickedPath));
	imageClicked->SetAlpha(0.85f);
	if (clickedPath != 0)
		delete[] clickedPath;

	button->SetImages(imageNormal, imageHighlighted, imageClicked);
	button->RegisterDrawFunction(imagebutton_draw);
}
