#pragma once

#include <cstdint>

struct Image
{
	uint32_t width;
	uint32_t height;
	uint32_t alpha;
	uint8_t* rgbPixels = nullptr;
	uint32_t* raster = nullptr;

	virtual ~Image();
	void LoadTIF(const char* path);
	void CreateErrorBitmap();
	void SetAlpha(float value);
	void Draw(int x, int y);
};
