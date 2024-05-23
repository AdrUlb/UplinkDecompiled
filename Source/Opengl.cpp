#include <App.hpp>
#include <Eclipse.hpp>
#include <GL/gl.h>
#include <Gci.hpp>
#include <Globals.hpp>

static void SetColour(const char* name)
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

static void clear_draw(int x, int y, int width, int height)
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

static void border_draw(Button* button)
{
	glBegin(GL_LINE_LOOP);
	glVertex2i(button->X, button->Y);
	glVertex2i(button->X + button->Width - 1, button->Y);
	glVertex2i(button->X + button->Width - 1, button->Y + button->Height - 1);
	glVertex2i(button->X, button->Y + button->Height - 1);
	glEnd();
}

static void text_draw(Button* button, bool highlighted, bool clicked)
{
	(void)button;
	(void)highlighted;
	(void)clicked;
	puts("TODO: implement text_draw()");
}

static void button_draw(Button* button, bool highlighted, bool clicked)
{
	(void)button;
	(void)highlighted;
	(void)clicked;
	UplinkAbort("TODO: implement button_draw()");
}

static void button_click(Button* button)
{
	(void)button;
	UplinkAbort("TODO: implement button_click()");
}

static void button_highlight(Button* button)
{
	(void)button;
	UplinkAbort("TODO: implement button_highlight()");
}

static void superhighlight_draw(Button* button)
{
	(void)button;
	UplinkAbort("TODO: implement superhighlight_draw()");
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

void textbutton_draw(struct Button* button, bool highlighted, bool clicked)
{
	UplinkAssert(button != nullptr);
	int32_t rax_2 = app->GetOptions()->GetOptionValue("graphics_screenheight");
	uint64_t height = ((uint64_t)button->Height);
	glScissor(button->X, ((-(button->Y) - height) + rax_2), ((uint64_t)button->Width), height);
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

void display()
{
	UplinkAbort("TODO: implement display()");
}

void mouse(GciMouseButton button, GciMouseEvent event, int x, int y)
{
	(void)button;
	(void)event;
	(void)x;
	(void)y;
	UplinkAbort("TODO: implement mouse()");
}

void mousemove(int x, int y)
{
	(void)x;
	(void)y;
	UplinkAbort("TODO: implement mousemove()");
}

void passivemouse(int x, int y)
{
	(void)x;
	(void)y;
	UplinkAbort("TODO: implement passivemouse()");
}

void keyboard(char keychar)
{
	(void)keychar;
	UplinkAbort("TODO: implement keyboard()");
}

void specialkeyboard(int keycode)
{
	(void)keycode;
	UplinkAbort("TODO: implement specialkeyboard()");
}

void idle()
{
	UplinkAbort("TODO: implement idle()");
}

void resize(int width, int height)
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
