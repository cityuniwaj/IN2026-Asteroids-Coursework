#ifndef __ANIMATIONMANAGER_H__
#define __ANIMATIONMANAGER_H__

#include "GameUtil.h"

class Image;
class Animation;

class AnimationManager
{
public:
	inline static AnimationManager& GetInstance(void)
	{
		static AnimationManager mInstance;
		return mInstance;
	}

	Animation* CreateAnimationFromFile(const string& n, const uint w, const uint h, const uint fw, const uint fh, const string& name);
	Animation* CreateAnimationFromImage(const string& name, const uint fw, const uint fh, Image* image);
	Animation* GetAnimationByName(const string& name);

private:
	AnimationManager() {} // Private constructor
	~AnimationManager() {} // Private destructor
	
	typedef map< string, Animation* > NamedAnimationMap;
	NamedAnimationMap mAnimationMap;
};

#endif