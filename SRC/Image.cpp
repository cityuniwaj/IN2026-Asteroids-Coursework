#include "GameUtil.h"
#include "Image.h"

#include "FreeImage.h"

using namespace std;

Image::Image()
	: mWidth(0),
	  mHeight(0),
	  mNumPixels(0),
	  mPixelData(NULL)
{
}

Image::Image(uint width, uint height)
	: mWidth(width),
	  mHeight(height),
	  mNumPixels(width*height)
{
	mPixelData = new uchar[4*mNumPixels];
}

Image::Image(uint width, uint height, const string& filename)
	: mWidth(width),
	  mHeight(height),
	  mNumPixels(width*height)
{
	mPixelData = new uchar[4*mNumPixels];
	LoadFile(filename);
}

Image::Image(Image* image, uint x, uint y, uint width, uint height)
	: mWidth(width),
	  mHeight(height),
	  mNumPixels(width*height)
{
	mPixelData = new uchar[4*mNumPixels];
	uchar *src_pixels = image->GetPixelData();
	uint dst = 0;
	
	for (uint j = 0; j < height; j++)
	{
		for (uint i = 0; i < width; i++)
		{
			mPixelData[dst++] = 255;
			mPixelData[dst++] = 0;
			mPixelData[dst++] = 0;
			mPixelData[dst++] = 255;
		}
	}
	dst = 0;
	
	for (uint j = 0; j < height; j++)
	{
		uint src = 4 * (x + ((y + j) * image->GetWidth()));
		for (uint i = 0; i < width; i++)
		{
			mPixelData[dst++] = src_pixels[src++];
			mPixelData[dst++] = src_pixels[src++];
			mPixelData[dst++] = src_pixels[src++];
			mPixelData[dst++] = src_pixels[src++];
		}
	}
}

Image::~Image()
{
	delete[] mPixelData;
}

void Image::SetTransparentColour(uchar r, uchar g, uchar b)
{
	for (uint i = 0; i < 4*mNumPixels; i += 4) {
		if (mPixelData[i] == r && mPixelData[i+1] == g && mPixelData[i+2] == b) {
			// Make pixel transparent
			mPixelData[i+3] = 0;
		} else {
			// Make pixel opaque
			mPixelData[i+3] = 255;
		}
	}
}

void Image::LoadFile(const string& filename)
{
	FREE_IMAGE_FORMAT format= FIF_UNKNOWN;
	FIBITMAP* pBitmap = nullptr;

	string file = "";
	file = file + filename;

	format = FreeImage_GetFileType(file.c_str());

	if(format == FIF_UNKNOWN)
		format = FreeImage_GetFIFFromFilename(file.c_str());

	if(format == FIF_UNKNOWN)
		return;

	if( FreeImage_FIFSupportsReading(format))
		pBitmap = FreeImage_Load(format, file.c_str());

	if(pBitmap == nullptr)
	{
		return;
	}

	// Check for 24 bits or 32 bits
	int bpp = FreeImage_GetBPP(pBitmap);

	//Free image has an inverted co-ordinate system. Fix as follows:
	// Flip the image vertically
	FreeImage_FlipVertical(pBitmap);
	// Flip the image horizontally
	FreeImage_FlipHorizontal(pBitmap);

	BYTE* pPixelData = FreeImage_GetBits(pBitmap);

	if(bpp == 24)
	{
		for(uint i = 0; i < mNumPixels*4; i +=4)
		{
			mPixelData[i+0] = pPixelData[i+0];
			mPixelData[i+1] = pPixelData[i+1];
			mPixelData[i+2] = pPixelData[i+2];
			mPixelData[i+3] = 255;
		}
	}
	else if(bpp == 32)
	{
		memcpy(mPixelData, pPixelData, sizeof(uchar) * mNumPixels * 4);
	}

	FreeImage_Unload(pBitmap);
}