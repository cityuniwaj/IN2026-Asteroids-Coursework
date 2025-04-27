#ifndef __WEAPONSPOWERUP_H__
#define __WEAPONSPOWERUP_H__

#include "GameObject.h"

class WeaponsPowerup : public GameObject
{
public:
    WeaponsPowerup(void);
    WeaponsPowerup(const GLVector3f& position);
    virtual ~WeaponsPowerup(void);

    virtual void Update(int t);
    bool CollisionTest(shared_ptr<GameObject> o);
    void OnCollision(const GameObjectList& objects);

protected:
    int mTimeToLive;
};

#endif