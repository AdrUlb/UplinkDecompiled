#include "Graphics.hpp"

#include <SDL/SDL.h>
#include <GL/gl.h>
#include "Util.hpp"
#include "../UplinkDecompiledTempGlobals.hpp"
#include "../UplinkDecompiledTempDefs.hpp"

constexpr uint GCI_FLAGS_0 = 1 << 0;
constexpr uint GCI_FLAGS_1 = 1 << 1;
constexpr uint GCI_FLAGS_2 = 1 << 2;
constexpr uint GCI_FLAGS_DEBUG = 1 << 3;

struct GciScreenMode
{
	int Width;
	int Height;
};

static char* GciInitGraphicsLibrary(uint flags)
{
	const auto debug = flags & GCI_FLAGS_DEBUG;

	if (debug)
		printf("Initialising SDL...");

	if (SDL_Init(SDL_INIT_VIDEO) == -1)
	{
		const auto format = "Could not initialize SDL: %s.";
		const auto sdlError = SDL_GetError();
		const auto formatLength = strlen(format);
		const auto sdlErrorLen = strlen(sdlError);
		const auto buffer = new char[sdlErrorLen + formatLength + 1];
		snprintf(buffer, sdlErrorLen + formatLength + 1, format, sdlError);
		return buffer;
	}

	if (debug)
		printf("done\n ");

	gGciIsInitGraphicsLibrary = true;
	return nullptr;
}

static GciScreenMode* GciGetClosestScreenMode(int width, int height)
{
	const auto screenMode = new GciScreenMode();
	screenMode->Width = width;
	screenMode->Height = height;

	auto modes = SDL_ListModes(nullptr, SDL_FULLSCREEN | SDL_OPENGL | SDL_HWSURFACE);

	if (!modes)
	{
		puts("SDL Resolutions: not using HW flag.");
		modes = SDL_ListModes(nullptr, SDL_FULLSCREEN | SDL_OPENGL);
	}

	auto smallestDiffW = -1;
	auto smallestDiffH = -1;
	for (size_t i = 0; modes[i]; i++)
	{
		const auto mode = modes[i];
		const auto diffW = abs(width - mode->w);
		const auto diffH = abs(height - mode->h);
		if ((smallestDiffW == -1 || smallestDiffH == -1) ||
			(diffH + diffW < smallestDiffH + smallestDiffW))
		{
			screenMode->Width = mode->w;
			screenMode->Height = mode->h;
			smallestDiffW = diffW;
			smallestDiffH = diffH;
		}

	}
	return screenMode;
}

void opengl_initialise(int argc, char* argv[])
{
	const auto options = gApp->GetOptions();
	const auto debug = options->IsOptionEqualTo("game_debugstart", 1);
	auto width = options->GetOptionValue("graphics_screenwidth");
	auto height = options->GetOptionValue("graphics_screenheight");
	const auto fullscreen = options->IsOptionEqualTo("graphics_fullscreen", 1);
	const auto depth = options->GetOptionValue("graphics_screendepth");
	const auto refresh = options->GetOptionValue("graphics_screenrefresh");
	const auto safemode = options->IsOptionEqualTo("graphics_safemode", 1);

	auto flags = GCI_FLAGS_0 | GCI_FLAGS_1;

	if (fullscreen && !safemode)
		flags |= GCI_FLAGS_2;

	if (debug)
		flags |= GCI_FLAGS_DEBUG;

	UplinkAssert(GciInitGraphicsLibrary(flags) == nullptr);

	auto screenMode = GciGetClosestScreenMode(width, height);
	if (width != screenMode->Width || height != screenMode->Height)
	{
		width = screenMode->Width;
		height = screenMode->Height;
		options->SetOptionValue("graphics_screenwidth", width);
		options->SetOptionValue("graphics_screenheight", height);
	}
	delete screenMode;

	UplinkAssert(GciInitGraphics("uplink", flags, width, height, depth, refresh, argc, argv) == nullptr);

	screenMode = (GciScreenMode*)GciGetCurrentScreenMode();
	if (width != screenMode->Width && height != screenMode->Height)
	{
		width = screenMode->Width;
		height = screenMode->Height;
		options->SetOptionValue("graphics_screenwidth", width);
		options->SetOptionValue("graphics_screenheight", height);
		printf("We actually ended up with %dx%d\n", width, height);
	}
	delete screenMode;

	if (debug)
		puts("Initialising OpenGL...");

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	glDisable(GL_DEPTH_TEST);

	glMatrixMode(GL_MODELVIEW);

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, 8448.0f);

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
	glBindTexture(GL_TEXTURE_2D, tex);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
	EclReset(width, height);
	EclRegisterClearDrawFunction(clear_draw);
	EclRegisterDefaultButtonCallbacks(button_draw, nullptr, button_click, button_highlight);
	EclRegisterSuperHighlightFunction(3, superhighlight_draw);

	const auto anims = options->GetOption("graphics_buttonanimations");
	if (anims && !anims->Value)
		EclDisableAnimations();

	const auto fastAnims = options->GetOption("graphics_fasterbuttonanimations");
	if (fastAnims && fastAnims->Value)
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