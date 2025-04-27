#ifndef __ASTEROID_H__
#define __ASTEROID_H__

#include "GameObject.h"

class Asteroid : public GameObject
{
public:
	Asteroid(void);
	~Asteroid(void);

	bool CollisionTest(shared_ptr<GameObject> o);
	void OnCollision(const GameObjectList& objects);
	void Bounce(shared_ptr<Asteroid> o);
	float GetSize() const { return mScale * 10.0f; }
	bool WasDestroyedByBullet() const { return mDestroyedByBullet; }

private:
	bool mDestroyedByBullet = false;
};

#endif