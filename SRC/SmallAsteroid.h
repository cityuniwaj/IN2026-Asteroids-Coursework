#ifndef __SMALLASTEROID_H__
#define __SMALLASTEROID_H__

#include "Asteroid.h"

class SmallAsteroid : public Asteroid
{
public:
    SmallAsteroid(void);
    SmallAsteroid(const GLVector3f& position, const GLVector3f& velocity);
    virtual ~SmallAsteroid(void);

    virtual void Update(int t) override;
    void OnCollision(const GameObjectList& objects);

protected:
    int mTimeToLive;
};

#endif