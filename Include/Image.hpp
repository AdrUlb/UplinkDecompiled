#pragma once

#include <cstdint>

class Image
{
	int _width;
	int _height;
	uint32_t _alpha;
	uint8_t* _rgbPixels = nullptr;
	uint32_t* _raster = nullptr;

public:
	Image();
	Image(Image* image);
	virtual ~Image();
	void LoadTIF(const char* path);
	void CreateErrorBitmap();
	void SetAlpha(float value);
	void Draw(int x, int y);
	void Scale(int newWidth, int newHeight);
	void FlipAroundH();
	int GetPixelR(int x, int y);
	int GetWidth();
	int GetHeight();
	const uint32_t* GetRaster();
};
