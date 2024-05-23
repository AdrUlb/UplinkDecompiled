#include <Gci.hpp>

#include <FTFace.h>
#include <FTGL/ftgl.h>
#include <SDL/SDL.h>
#include <Util.hpp>
#include <cstdio>
#include <cstdlib>
#include <map>

static SDL_Surface* screen = nullptr;

static bool _GciIsInitGraphicsLibrary = false;
static bool finished = false;

static DisplayFunc gciDisplayHandlerP = nullptr;
static MouseFunc gciMouseHandlerP = nullptr;
static MotionFunc gciMotionHandlerP = nullptr;
static PassiveMotionFunc gciPassiveMotionHandlerP = nullptr;
static KeyboardFunc gciKeyboardHandlerP = nullptr;
static KeyboardSpecialFunc gciSpecialHandlerP = nullptr;
static IdleFunc gciIdleHandlerP = nullptr;
static ReshapeFunc gciReshapeHandlerP = nullptr;

static bool gci_truetypeenabled = false;
static int gci_defaultfont = 6;

static std::map<int, FTGLBitmapFont*> fonts;

void GciDisplayFunc(DisplayFunc func)
{
	gciDisplayHandlerP = func;
}

void GciMouseFunc(MouseFunc func)
{
	gciMouseHandlerP = func;
}

void GciMotionFunc(MotionFunc func)
{
	gciMotionHandlerP = func;
}

void GciPassiveMotionFunc(PassiveMotionFunc func)
{
	gciPassiveMotionHandlerP = func;
}

void GciKeyboardFunc(KeyboardFunc func)
{
	gciKeyboardHandlerP = func;
}

void GciSpecialFunc(KeyboardSpecialFunc func)
{
	gciSpecialHandlerP = func;
}

void GciIdleFunc(IdleFunc func)
{
	gciIdleHandlerP = func;
}

void GciReshapeFunc(ReshapeFunc func)
{
	gciReshapeHandlerP = func;
}

void GciRestoreScreenSize()
{
	if (_GciIsInitGraphicsLibrary && SDL_WasInit(SDL_INIT_VIDEO))
	{
		int isDoubleBuffer = 0;
		if (SDL_GL_GetAttribute(SDL_GL_DOUBLEBUFFER, &isDoubleBuffer) == 0 && isDoubleBuffer)
			SDL_GL_SwapBuffers();

		SDL_QuitSubSystem(SDL_INIT_VIDEO);
	}

	_GciIsInitGraphicsLibrary = false;
	finished = true;
}

const char* GciInitGraphicsLibrary(GciInitFlags flags)
{
	const auto debug = flags.Debug;

	if (debug)
		printf("Initialising SDL...");

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		const auto format = "Could not initialize SDL: %s.";
		const auto sdlError = SDL_GetError();

		const auto byteCount = strlen(format) + strlen(sdlError) + 1;

		const auto errorBuffer = new char[byteCount];

		snprintf(errorBuffer, byteCount, format, sdlError);

		return errorBuffer;
	}

	if (debug)
		printf("done\n ");

	_GciIsInitGraphicsLibrary = 1;
	return nullptr;
}

const char* GciInitGraphics(const char* title, GciInitFlags flags, int width, int height, int depth)
{
	if (flags.UnknownFlag0)
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	const auto videoInfo = SDL_GetVideoInfo();

	if (depth == -1)
		depth = videoInfo->vfmt->BitsPerPixel;

	if (flags.UnknownFlag1)
	{
		if (depth == 24 || depth == 32)
		{
			SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
			SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
			SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
		}
		else
		{
			depth = 16;
			SDL_GL_SetAttribute(SDL_GL_RED_SIZE, depth / 3);
			SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, depth / 3);
			SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, depth / 3);
		}
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
	}

	uint32_t videoModeFlags = SDL_OPENGL;

	if (flags.Fullscreen)
		videoModeFlags |= SDL_FULLSCREEN;

	const auto closestDepth = SDL_VideoModeOK(width, height, depth, videoModeFlags);
	if (closestDepth == 0)
	{
		printf("Warning, no available video mode for width: %d, height:%d, flags:%d\n", width, height, videoModeFlags);
	}
	else if (depth != closestDepth)
	{
		printf("Warning, difference in depth between the video mode requested %d and the closest available %d for width: %d, height:%d, flags:%d\n", depth, closestDepth, width,
			   height, videoModeFlags);

		depth = closestDepth;

		if (depth == 24 || depth == 32)
		{
			SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
			SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
			SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
		}
		else
		{
			SDL_GL_SetAttribute(SDL_GL_RED_SIZE, depth / 3);
			SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, depth / 3);
			SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, depth / 3);
		}
	}
	if (flags.Debug)
		printf("SDL is now opening a %dx%d window in %d depth ...", width, height, depth);

	screen = SDL_SetVideoMode(width, height, depth, videoModeFlags);

	if (screen == nullptr)
	{
		const auto format = "Could not initialize SDL Video: %s.";
		const auto sdlError = SDL_GetError();

		const auto byteCount = strlen(format) + strlen(sdlError) + 1;

		const auto errorBuffer = new char[byteCount];

		snprintf(errorBuffer, byteCount, format, sdlError);

		return errorBuffer;
	}

	if (flags.Debug)
	{
		printf("done\n ");
		printf("SDL is now changing the window caption and diverse settings ...");
	}

	SDL_WM_SetCaption(title, nullptr);
	SDL_EnableUNICODE(true);
	SDL_EnableKeyRepeat(500, 30);

	if (flags.Debug)
		printf("done\n ");

	return nullptr;
}

int* GciGetClosestScreenMode(int width, int height)
{
	const auto ret = new int[2];
	ret[0] = width;
	ret[1] = height;

	auto modes = SDL_ListModes(0, SDL_FULLSCREEN | SDL_OPENGL | SDL_HWSURFACE);

	if (modes == nullptr)
	{
		puts("SDL Resolutions: not using HW flag.");
		modes = SDL_ListModes(nullptr, SDL_FULLSCREEN | SDL_OPENGL);
	}

	int minDist = -1;

	for (size_t i = 0; modes[i] != nullptr; i++)
	{
		auto mode = modes[i];
		int dx = mode->w - width;
		int dy = mode->h - height;
		int diff = (dx * dx) + (dy * dy);

		if (minDist == -1 || diff < minDist)
		{
			ret[0] = mode->w;
			ret[1] = mode->h;
			minDist = diff;
		}
	}

	return ret;
}

int* GciGetCurrentScreenMode()
{
	const auto ret = new int[2];
	const auto surface = SDL_GetVideoSurface();
	ret[0] = surface->w;
	ret[1] = surface->h;
	return ret;
}

void GciEnableTrueTypeSupport()
{
	gci_truetypeenabled = 1;
}

void GciDisableTrueTypeSupport()
{
	gci_truetypeenabled = 0;
}

bool GciRegisterTrueTypeFont()
{
	return true;
}

bool GciUnregisterTrueTypeFont()
{
	return true;
}

void GciSetDefaultFont(int index)
{
    gci_defaultfont = index;
}

void GciDeleteTrueTypeFont(int index)
{
	if (fonts[index] == nullptr)
		return;

	delete fonts[index];
	fonts[index] = nullptr;
}

bool GciLoadTrueTypeFont(int index, const char* name, const char* path, int size)
{
	(void)name;
	if (!gci_truetypeenabled)
	{
		puts("GciLoadTrueTypeFont called, but truetypes are not enabled");
		return false;
	}

	const auto font = new FTGLBitmapFont(path);

	if (font->Error() != FT_Err_Ok || !font->FaceSize(size, 96))
	{
		delete font;
		return false;
	}

	GciDeleteTrueTypeFont(index);
	fonts[index] = font;
	return true;
}
