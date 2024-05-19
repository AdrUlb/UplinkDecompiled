#include <App.hpp>
#include <Eclipse.hpp>
#include <GL/gl.h>
#include <Gci.hpp>
#include <Globals.hpp>

void opengl_initialise()
{
	const auto debug = app->GetOptions()->IsOptionEqualTo("game_debugstart", 1);

	auto width = app->GetOptions()->GetOptionValue("graphics_screenwidth");
	auto height = app->GetOptions()->GetOptionValue("graphics_screenheight");
	const auto depth = app->GetOptions()->GetOptionValue("graphics_screendepth");
	const auto fullscreen = app->GetOptions()->IsOptionEqualTo("graphics_fullscreen", 1);
	const auto safemode = app->GetOptions()->IsOptionEqualTo("graphics_safemode", 1);

	GciInitFlags flags{.UnknownFlag0 = true, .UnknownFlag1 = true, .Fullscreen = fullscreen && !safemode};

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
	delete screenMode;

	if (const auto errorMessage = GciInitGraphics("uplink", flags, width, height, depth); errorMessage != nullptr)
		UplinkAbort(errorMessage);

	const auto actualScreenMode = GciGetCurrentScreenMode();
	if (actualScreenMode[0] != width || actualScreenMode[1] != height)
	{
		printf("We actually ended up with %dx%d\n", actualScreenMode[0], actualScreenMode[1]);
		app->GetOptions()->SetOptionValue("graphics_screenwidth", actualScreenMode[0]);
		app->GetOptions()->SetOptionValue("graphics_screenheight", actualScreenMode[1]);
	}

	delete actualScreenMode;

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

	UplinkAbort("TODO: implement opengl_initialise()");

	/*EclReset();
	EclRegisterClearDrawFunction(clear_draw);
	EclRegisterDefaultButtonCallbacks(button_draw, 0, button_click, button_highlight);
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
		printf("done\n ");*/
}
