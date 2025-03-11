#include "Image.h"
#include "ImageManager.h"

Image* ImageManager::CreateImageFromFile(const string& name, const uint width, const uint height, const string& filename)
{
	Image* image = new Image(width, height, filename);
	mImageMap.insert(NamedImageMap::value_type(name, image));
	return image;
}

Image* ImageManager::CreateImageFromImage(const string& name, Image* image, const uint x, const uint y, const uint w, const uint h)
{
	Image* new_image = new Image(image, x, y, w, h);
	mImageMap.insert(NamedImageMap::value_type(name, new_image));
	return new_image;
}

Image* ImageManager::GetImageByName(const string& name)
{
	NamedImageMap::iterator it = mImageMap.find(name);
	return (it != mImageMap.end()) ? it->second : 0;
}