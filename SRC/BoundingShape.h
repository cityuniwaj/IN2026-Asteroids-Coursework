#ifndef __BOUNDINGSHAPE_H__
#define __BOUNDINGSHAPE_H__

#include "GameUtil.h"
#include "GameObjectType.h"
#include "GameObject.h"

class BoundingShape
{
public:
	BoundingShape(char const * const type_name) : mType(type_name) {}
	BoundingShape(char const * const type_name, shared_ptr<GameObject> o)
		: mType(type_name), mGameObject(o) {}

	virtual bool CollisionTest(shared_ptr<BoundingShape> bshape) { return false; }
	
	const GameObjectType& GetType() const { return mType; }

	virtual void SetGameObject(shared_ptr<GameObject> o) { mGameObject = o; }
	virtual shared_ptr<GameObject> GetGameObject() { return mGameObject.lock(); }



protected:
	GameObjectType mType;
	weak_ptr<GameObject> mGameObject;
};

#endif