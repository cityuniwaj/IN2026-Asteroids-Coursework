#ifndef __EXPLODINGBULLET_H__
#define __EXPLODINGBULLET_H__

#include "GameObject.h"
#include "GameObjectType.h"

class ExplodingBullet : public GameObject
{
public:
    ExplodingBullet();
    ExplodingBullet(GLVector3f p, GLVector3f v, GLfloat h, GLfloat ttl);
    virtual ~ExplodingBullet(void);

    virtual void Update(int t);
    bool CollisionTest(shared_ptr<GameObject> o);
    void OnCollision(const GameObjectList& objects);

    void Explode();
    void CreateExplosionEffect();

protected:
    int mTimeToLive;
    bool mExploded;
};

#endif