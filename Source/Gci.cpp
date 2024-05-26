#include <Gci.hpp>

#include <FTFace.h>
#include <FTGL/ftgl.h>
#include <GL/gl.h>
#include <SDL/SDL.h>
#include <Util.hpp>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <list>
#include <map>
#include <sys/time.h>

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

static bool displayDamaged = false;

struct TimerEvent
{
	GciTimerCallback callback;
	int callbackArg;
	uint32_t fireTime;
};

static std::list<TimerEvent*> timerEvents;

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
		printf("Warning, difference in depth between the video mode requested %d and the closest available %d for width: %d, height:%d, "
			   "flags:%d\n",
			   depth, closestDepth, width, height, videoModeFlags);

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
	font->GlyphLoadFlags(FT_LOAD_LINEAR_DESIGN);

	if (font->Error() != FT_Err_Ok || !font->FaceSize(size, 96))
	{
		delete font;
		return false;
	}

	GciDeleteTrueTypeFont(index);
	fonts[index] = font;
	return true;
}

void GciTimerFunc(int delay, GciTimerCallback callback, int arg)
{
	const auto rax = new TimerEvent();
	rax->callback = callback;
	rax->callbackArg = arg;
	rax->fireTime = SDL_GetTicks() + delay;
	timerEvents.push_back(rax);
}

size_t _GciGetAccurateTime()
{
	static bool initted = false;
	static timeval startTime;

	timeval thisTime;
	gettimeofday(&thisTime, nullptr);

	if (!initted)
	{
		initted = true;
		startTime = thisTime;
		return 0;
	}

	auto rcx_1 = thisTime.tv_usec - startTime.tv_usec;
	if (thisTime.tv_usec - startTime.tv_usec < 0)
	{
		thisTime.tv_sec--;
		rcx_1 += 1'000'000;
	}
	return (rcx_1 / 1'000) + ((thisTime.tv_sec - startTime.tv_sec) * 1'000);
}

void GciMainLoop()
{
	_GciGetAccurateTime();

	finished = false;

	while (!finished)
	{
		if (gciDisplayHandlerP != nullptr)
			gciDisplayHandlerP();

		SDL_Event event;
		while (SDL_PollEvent(&event) != 0 && !finished)
		{
			switch (event.type)
			{
				case SDL_KEYDOWN:
					puts("TODO: handle SDL_KEYDOWN");
					break;
				case SDL_MOUSEMOTION:
					if (gciMotionHandlerP != nullptr)
						gciMotionHandlerP(event.motion.x, event.motion.y);
					if (gciPassiveMotionHandlerP != nullptr)
						gciPassiveMotionHandlerP(event.motion.x, event.motion.y);
					break;
				case SDL_MOUSEBUTTONDOWN:
					puts("TODO: handle SDL_MOUSEBUTTONDOWN");
					break;
				case SDL_MOUSEBUTTONUP:
					puts("TODO: handle SDL_MOUSEBUTTONUP");
					break;
				case SDL_QUIT:
					finished = true;
					break;
				case SDL_VIDEORESIZE:
					puts("TODO: handle SDL_VIDEORESIZE");
					break;
				case SDL_VIDEOEXPOSE:
					displayDamaged = true;
					break;
				default:
					break;
			}
		}

		if (finished)
			break;

		for (auto it = timerEvents.begin(); it != timerEvents.end();)
		{
			auto timerEvent = *it;
			if (SDL_GetTicks() >= timerEvent->fireTime)
			{
				it = timerEvents.erase(it);
				timerEvent->callback(timerEvent->callbackArg);
				delete timerEvent;
				continue;
			}
			it++;
		}

		if (gciIdleHandlerP != nullptr)
			gciIdleHandlerP();
	}
}

bool GciAppVisible()
{
	return (SDL_GetAppState() & SDL_APPACTIVE) != 0;
}

void GciSwapBuffers()
{
	SDL_GL_SwapBuffers();
	displayDamaged = false;
}

static int GciFallbackTextWidth(const char* text)
{
	static auto inside = false;

	if (inside)
		abort();

	inside = true;
	const auto ret = GciTextWidth(text, gci_defaultfont);
	inside = false;
	return ret;
}

static void GciFallbackDrawText(int x, int y, const char* text)
{
	static auto inside = false;

	if (inside)
		abort();

	inside = true;
	GciDrawText(x, y, text, gci_defaultfont);
	inside = false;
}

int GciTextWidth(char const* text, int font)
{
	if (fonts[font] == nullptr)
		return GciFallbackTextWidth(text);

	float llx, lly, llz, urx, ury, urz;
	fonts[font]->BBox(text, llx, lly, llz, urx, ury, urz);
	return (int)(fabs(urx - llx) + 0.5);
}

int GciTextWidth(const char* text)
{
	return GciTextWidth(text, gci_defaultfont);
}

void GciDrawText(int x, int y, const char* text, int font)
{
	if (!gci_truetypeenabled || fonts[font] == nullptr)
	{
		GciFallbackDrawText(x, y, text);
		return;
	}

	glRasterPos2i(x, y);
	fonts[font]->Render(text);
}

void GciDrawText(int x, int y, const char* text)
{
	GciDrawText(x, y, text, gci_defaultfont);
}
