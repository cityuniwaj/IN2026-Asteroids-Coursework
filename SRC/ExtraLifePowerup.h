#ifndef __EXTRALIFEPOWERUP_H__
#define __EXTRALIFEPOWERUP_H__

#include "GameObject.h"

class ExtraLifePowerup : public GameObject
{
public:
    ExtraLifePowerup(void);
    ExtraLifePowerup(const GLVector3f& position);
    virtual ~ExtraLifePowerup(void);

    virtual void Update(int t);
    bool CollisionTest(shared_ptr<GameObject> o);
    void OnCollision(const GameObjectList& objects);

protected:
    int mTimeToLive;
};

#endif