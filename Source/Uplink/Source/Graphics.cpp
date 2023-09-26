#include <Graphics.hpp>

#include <SDL/SDL.h>
#include <GL/gl.h>
#include <Util.hpp>
#include "../UplinkDecompiledTempGlobals.hpp"
#include "../UplinkDecompiledTempDefs.hpp"

constexpr uint GCI_FLAGS_DOUBLEBUFFER = 1 << 0;
constexpr uint GCI_FLAGS_1 = 1 << 1;
constexpr uint GCI_FLAGS_FULLSCREEN = 1 << 2;
constexpr uint GCI_FLAGS_DEBUG = 1 << 3;

struct GciScreenMode
{
	int Width;
	int Height;

public:
	inline GciScreenMode() {}
	inline GciScreenMode(int width, int height) : Width(width), Height(height) {}
};

static void EclDirtyClear()
{
	auto index = gButtons.Size() - 1;

	while (index >= 0)
	{
		if (gButtons.ValidIndex(index))
			gButtons[index]->Dirty = false;

		index--;
	}
}

static void EclReset(int width, int height)
{
	(void)width;
	(void)height;

	if (gCurrentHighlight)
		delete[] gCurrentHighlight;

	if (gCurrentClick)
		delete[] gCurrentClick;

	gCurrentHighlight = nullptr;
	gCurrentClick = nullptr;

	while (gButtons.ValidIndex(0))
		EclRemoveButton(gButtons.GetData(0)->Name);

	while (const auto b = gEditableButtons.GetData(0))
	{
		if (!b)
			break;

		gEditableButtons.RemoveData(0);

		if (b)
			delete[] b;
	}
	gButtons.Empty();
	gEditableButtons.Empty();
	gSuperhighlightBorderWidth = 0;
	EclDirtyClear();
	return;
}

static void EclRegisterClearDrawFunction(ClearDrawFunc func)
{
	gClearDrawFunc = func;
}

static void EclRegisterDefaultButtonCallbacks(ButtonDrawFunc drawFunc, ButtonMouseUpFunc mouseUpFunc,
	ButtonMouseDownFunc mouseDownFunc, ButtonMouseMoveFunc mouseMoveFunc)
{
	gDefaultDrawFunc = drawFunc;
	gDefaultMouseUpFunc = mouseUpFunc;
	gDefaultMouseDownFunc = mouseDownFunc;
	gDefaultMouseMoveFunc = mouseMoveFunc;
}

static void EclRegisterSuperHighlightFunction(int borderWidth, ButtonDrawFunc func)
{
	gSuperhighlightBorderWidth = borderWidth;
	gSuperhighlightDrawFunc = func;
}

static void EclDisableAnimations()
{
	gAnimsEnabled = false;
}

static void EclEnableFasterAnimations(double speed)
{
	gAnimsFasterSpeed = speed;
	gAnimsFasterEnabled = true;
}

static char* GciInitGraphicsLibrary(uint flags)
{
	const auto debug = flags & GCI_FLAGS_DEBUG;

	if (debug)
		printf("Initialising SDL...");

	if (SDL_Init(SDL_INIT_VIDEO) == -1)
	{
		const auto format = "Could not initialize SDL: %s.";
		const auto sdlError = SDL_GetError();

		const auto length = strlen(format) + strlen(sdlError);

		const auto buffer = new char[length + 1];
		snprintf(buffer, length, format, sdlError);
		return buffer;
	}

	if (debug)
		printf("done\n ");

	gGciIsInitGraphicsLibrary = true;
	return nullptr;
}

static char* GciInitGraphics(const char* title, int flags, int width, int height, int bpp, int refresh, int argc, char* argv[])
{
	(void)refresh;
	(void)argc;
	(void)argv;
	if (flags & GCI_FLAGS_DOUBLEBUFFER)
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	const auto videoInfo = SDL_GetVideoInfo();

	if (bpp == -1)
		bpp = videoInfo->vfmt->BitsPerPixel;

	if (flags & GCI_FLAGS_1)
	{
		if (bpp == 24 || bpp == 32)
		{
			SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
			SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
			SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
		}
		else
		{
			bpp = 16;
			SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 5);
			SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 5);
			SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 5);
		}
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
	}

	Uint32 videoFlags = SDL_OPENGL;

	if (flags & GCI_FLAGS_FULLSCREEN)
		videoFlags |= SDL_FULLSCREEN;

	const auto actualBpp = SDL_VideoModeOK(width, height, bpp, videoFlags);
	if (actualBpp == 0)
	{
		printf("Warning, no available video mode for width: %d, height:%d, flags:%d\n", width, height, videoFlags);
	}
	else if (bpp != actualBpp)
	{
		printf("Warning, difference in depth between the video mode requested %d and the closest availab le %d for width: %d, height:%d, flags:%d\n",
			bpp, actualBpp, width, height, videoFlags);

		bpp = actualBpp;
		if (actualBpp == 24 || (actualBpp == 32))
		{
			SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
			SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
			SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
		}
		else if (actualBpp == 16)
		{
			SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 5);
			SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 5);
			SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 5);
			bpp = 0x10;
		}
		else
		{
			const auto bitsPerChannel = actualBpp / 3;
			SDL_GL_SetAttribute(SDL_GL_RED_SIZE, bitsPerChannel);
			SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, bitsPerChannel);
			SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, bitsPerChannel);
		}
	}

	if (flags & GCI_FLAGS_DEBUG)
		printf("SDL is now opening a %dx%d window in %d depth ...", width, height, bpp);

	gScreen = SDL_SetVideoMode(width, height, bpp, videoFlags);
	if (!gScreen)
	{
		const auto format = "Could not initialize SDL Video: %s.";
		const auto sdlError = SDL_GetError();

		const auto length = strlen(format) + strlen(sdlError);

		const auto buffer = new char[length + 1];
		snprintf(buffer, length, format, sdlError);
		return buffer;
	}

	if (flags & GCI_FLAGS_DEBUG)
	{
		printf("done\n ");
		printf("SDL is now changing the window caption and diverse settings ...");
	}

	SDL_WM_SetCaption(title, nullptr);
	SDL_EnableUNICODE(true);
	SDL_EnableKeyRepeat(500, 30);

	if (flags & GCI_FLAGS_DEBUG)
		printf("done\n ");

	return nullptr;
}

static GciScreenMode* GciGetClosestScreenMode(int width, int height)
{
	const auto screenMode = new GciScreenMode(width, height);

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

GciScreenMode* GciGetCurrentScreenMode()
{
	const auto surface = SDL_GetVideoSurface();
	return new GciScreenMode(surface->w, surface->h);
}

void GciRestoreScreenSize()
{
	if (gGciIsInitGraphicsLibrary && SDL_WasInit(SDL_INIT_VIDEO) != 0)
	{
		int doubleBuffer;
		if (SDL_GL_GetAttribute(SDL_GL_DOUBLEBUFFER, &doubleBuffer) == 0 && doubleBuffer)
			SDL_GL_SwapBuffers();
		SDL_QuitSubSystem(SDL_INIT_VIDEO);
	}

	gGciIsInitGraphicsLibrary = false;
	gGciFinished = true;
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

	auto flags = GCI_FLAGS_DOUBLEBUFFER | GCI_FLAGS_1;

	if (fullscreen && !safemode)
		flags |= GCI_FLAGS_FULLSCREEN;

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

	screenMode = GciGetCurrentScreenMode();
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
