#include <Image.hpp>

#include <GL/gl.h>
#include <GL/glu.h>
#include <tiffio.h>

Image::~Image()
{
	if (raster != nullptr)
		delete[] raster;

	if (rgbPixels != nullptr)
		delete[] rgbPixels;
}

void Image::LoadTIF(const char* path)
{
	TIFFRGBAImage img;

	const auto tif = TIFFOpen(path, "r");
	if (tif == nullptr)
	{
		printf("GUCCI Error - failed to load TIF %s\n", path);
		CreateErrorBitmap();
		return;
	}

	char emsg[1024];
	// TODO: don't assume that TIFFRGBAImageBegin() will always succeed
	TIFFRGBAImageBegin(&img, tif, 0, emsg);
	const auto raster = new uint32_t[img.width * img.height];
	// TODO: don't assume that TIFFRGBAImageGet() will always succeed
	TIFFRGBAImageGet(&img, raster, img.width, img.height);

	width = img.width;
	height = img.height;

	if (rgbPixels != nullptr)
	{
		delete[] rgbPixels;
		this->rgbPixels = nullptr;
	}

	if (this->raster != 0)
		delete[] this->raster;
	this->raster = raster;

	TIFFRGBAImageEnd(&img);
	TIFFClose(tif);
}

void Image::CreateErrorBitmap()
{
	width = 32;
	height = 32;

	const auto raster = new uint32_t[width * height];

	for (uint32_t x = 0; x < width; x++)
	{
		for (uint32_t y = 0; y < height; y++)
		{
			if (x != 0 && y != 0 && x != width - 1 && y != height - 1 && x != y && x + y != width)
			{
				raster[x + (y * width)] = 0xFF000000;
				continue;
			}
			raster[x + (y * width)] = 0xFFFFFFFF;
		}
	}

	if (rgbPixels != nullptr)
	{
		delete[] rgbPixels;
		rgbPixels = nullptr;
	}

	if (this->raster != 0)
		delete[] this->raster;
	this->raster = raster;
}

void Image::SetAlpha(float value)
{
	alpha = value * 256.0f;
	if (raster == nullptr)
		return;

	for (uint32_t x = 0; x < width; x++)
		for (uint32_t y = 0; y < height; y++)
			raster[x + (y * width)] = (raster[x + (y * width)] & 0x00FFFFFF) | (alpha << 24);
}

#include <cstring>

void Image::Draw(int x, int y)
{
	if (raster == 0)
		return;

	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glDisable(GL_BLEND);
	glRasterPos2i(x, y + height);
	glDrawPixels(width, height, GL_RGBA, GL_UNSIGNED_BYTE, raster);
	glPopAttrib();
}

void Image::Scale(int32_t newWidth, int32_t newHeight)
{
	if (this->raster == nullptr)
		return;

	const auto newRaster = new uint32_t[newWidth * newHeight];
	gluScaleImage(GL_RGBA, width, height, GL_UNSIGNED_BYTE, raster, newWidth, newHeight, GL_UNSIGNED_BYTE, newRaster);

	if (rgbPixels != nullptr)
	{
		delete[] rgbPixels;
		rgbPixels = nullptr;
	}

	delete[] raster;

	raster = newRaster;
	width = newWidth;
	height = newHeight;
}

void Image::FlipAroundH()
{
	if (raster == nullptr)
		return;

	const auto newRaster = new uint32_t[width * height];

	const auto bytesPerRow = width * 4;
	for (uint32_t i = 0; i < height; i++)
	{
		const auto offset = i * bytesPerRow;
		memcpy((char*)newRaster + (height * bytesPerRow) - bytesPerRow - offset, (char*)raster + offset, bytesPerRow);
	}

	if (rgbPixels != nullptr)
	{
		delete[] rgbPixels;
		rgbPixels = nullptr;
	}

	delete[] raster;
	raster = newRaster;
}
