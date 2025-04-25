#ifndef __INVINCIBILITYPOWERUP_H__
#define __INVINCIBILITYPOWERUP_H__

#include "GameObject.h"

class InvincibilityPowerup : public GameObject
{
public:
    InvincibilityPowerup(void);
    InvincibilityPowerup(const GLVector3f& position);
    virtual ~InvincibilityPowerup(void);

    virtual void Update(int t);
    bool CollisionTest(shared_ptr<GameObject> o);
    void OnCollision(const GameObjectList& objects);

protected:
    int mTimeToLive;
};

#endif