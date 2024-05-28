#pragma once

#include <cstdint>

struct Image
{
	int width;
	int height;
	uint32_t alpha;
	uint8_t* rgbPixels = nullptr;
	uint32_t* raster = nullptr;

	virtual ~Image();
	void LoadTIF(const char* path);
	void CreateErrorBitmap();
	void SetAlpha(float value);
	void Draw(int x, int y);
	void Scale(int32_t newWidth, int32_t newHeight);
	void FlipAroundH();
	int GetPixelR(int x, int y);
};
