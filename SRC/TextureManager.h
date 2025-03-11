#ifndef __TEXTUREMANAGER_H__
#define __TEXTUREMANAGER_H__

#include "GameUtil.h"

class Image;
class Texture;

class TextureManager
{
public:
	inline static TextureManager& GetInstance(void)
	{
		static TextureManager mInstance;
		return mInstance;
	}

	Texture* CreateTextureFromFile(const string& n, const uint w, const uint h, const string& filename);
	Texture* CreateTextureFromImage(const string& name, Image* image);
	Texture* GetTextureByName(const string& name);

private:
	TextureManager() {} // Private constructor
	~TextureManager() {} // Private destructor
	
	typedef map< string, Texture* > NamedTextureMap;
	NamedTextureMap mTextureMap;
};

#endif