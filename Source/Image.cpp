#include <Image.hpp>

#include <GL/gl.h>
#include <GL/glu.h>
#include <cstring>
#include <tiffio.h>

Image::Image() {}

Image::Image(Image* image)
{
	_width = image->_width;
	_height = image->_height;
	_alpha = image->_alpha;

	if (image->_raster != nullptr)
	{
		_raster = new uint32_t[_width * _height];
		memcpy(_raster, image->_raster, _width * _height * sizeof(uint32_t));
	}

	_rgbPixels = nullptr;
	if (image->_rgbPixels != nullptr)
	{
		_rgbPixels = new uint8_t[_width * _height * 3];
		memcpy(_rgbPixels, image->_rgbPixels, _width * _height * 3);
	}
}

Image::~Image()
{
	if (_raster != nullptr)
		delete[] _raster;

	if (_rgbPixels != nullptr)
		delete[] _rgbPixels;
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

	_width = img.width;
	_height = img.height;

	if (_rgbPixels != nullptr)
	{
		delete[] _rgbPixels;
		this->_rgbPixels = nullptr;
	}

	if (this->_raster != 0)
		delete[] this->_raster;
	this->_raster = raster;

	TIFFRGBAImageEnd(&img);
	TIFFClose(tif);
}

void Image::CreateErrorBitmap()
{
	_width = 32;
	_height = 32;

	const auto raster = new uint32_t[_width * _height];

	for (auto x = 0; x < _width; x++)
	{
		for (auto y = 0; y < _height; y++)
		{
			if (x != 0 && y != 0 && x != _width - 1 && y != _height - 1 && x != y && x + y != _width)
			{
				raster[x + (y * _width)] = 0xFF000000;
				continue;
			}
			raster[x + (y * _width)] = 0xFFFFFFFF;
		}
	}

	if (_rgbPixels != nullptr)
	{
		delete[] _rgbPixels;
		_rgbPixels = nullptr;
	}

	if (this->_raster != 0)
		delete[] this->_raster;
	this->_raster = raster;
}

void Image::SetAlpha(float value)
{
	_alpha = value * 256.0f;
	if (_raster == nullptr)
		return;

	for (auto x = 0; x < _width; x++)
		for (auto y = 0; y < _height; y++)
			_raster[x + (y * _width)] = (_raster[x + (y * _width)] & 0x00FFFFFF) | (_alpha << 24);
}

#include <cstring>

void Image::Draw(int x, int y)
{
	if (_raster == 0)
		return;

	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glDisable(GL_BLEND);
	glRasterPos2i(x, y + _height);
	glDrawPixels(_width, _height, GL_RGBA, GL_UNSIGNED_BYTE, _raster);
	glPopAttrib();
}

void Image::Scale(int newWidth, int newHeight)
{
	if (this->_raster == nullptr)
		return;

	const auto newRaster = new uint32_t[newWidth * newHeight];
	gluScaleImage(GL_RGBA, _width, _height, GL_UNSIGNED_BYTE, _raster, newWidth, newHeight, GL_UNSIGNED_BYTE, newRaster);

	if (_rgbPixels != nullptr)
	{
		delete[] _rgbPixels;
		_rgbPixels = nullptr;
	}

	delete[] _raster;

	_raster = newRaster;
	_width = newWidth;
	_height = newHeight;
}

void Image::FlipAroundH()
{
	if (_raster == nullptr)
		return;

	const auto newRaster = new uint32_t[_width * _height];

	const auto bytesPerRow = _width * 4;
	for (auto i = 0; i < _height; i++)
	{
		const auto offset = i * bytesPerRow;
		memcpy((char*)newRaster + (_height * bytesPerRow) - bytesPerRow - offset, (char*)_raster + offset, bytesPerRow);
	}

	if (_rgbPixels != nullptr)
	{
		delete[] _rgbPixels;
		_rgbPixels = nullptr;
	}

	delete[] _raster;
	_raster = newRaster;
}

int Image::GetPixelR(int x, int y)
{
	if (_raster == nullptr || x < 0 || y < 0 || x >= _width || y >= _height)
		return -1;

	return _raster[x + (y * _width)] & 0xFF;
}

int Image::GetWidth()
{
	return _width;
}

int Image::GetHeight()
{
	return _height;
}

const uint32_t* Image::GetRaster()
{
	return _raster;
}
