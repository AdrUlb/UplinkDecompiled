#pragma once

#include <cstdint>

struct Image
{
	uint32_t width;
	uint32_t height;
	uint32_t alpha;
	uint8_t* rgbPixels;
	uint8_t* raster;
};
