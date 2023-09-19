#include "Options.hpp"

#include <cstring>

Options::Options()
{
	strncpy(themeName, "graphics", OPTIONS_THEMENAME_MAX);
}
