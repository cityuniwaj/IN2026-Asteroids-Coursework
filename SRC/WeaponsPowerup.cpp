#include "WeaponsPowerup.h"
#include "BoundingSphere.h"
#include "GameUtil.h"
#include "GameWorld.h"
#include "Spaceship.h"

WeaponsPowerup::WeaponsPowerup(void) : GameObject("WeaponsPowerup")
{
    mPosition.x = 0.0f;
    mPosition.y = 0.0f;
    mPosition.z = 0.0f;
    mTimeToLive = 20000;
}

WeaponsPowerup::WeaponsPowerup(const GLVector3f& position) : GameObject("WeaponsPowerup")
{
    mPosition = position;
    mAngle = 0;
    mRotation = 0;
    mTimeToLive = 20000;
}

WeaponsPowerup::~WeaponsPowerup(void)
{
}

void WeaponsPowerup::Update(int t)
{
    GameObject::Update(t);
    mTimeToLive -= t;
    if (mTimeToLive <= 0)
    {
        //if (mWorld) mWorld->FlagForRemoval(GetThisPtr());
        return;
    }
}

bool WeaponsPowerup::CollisionTest(shared_ptr<GameObject> o)
{
    if (o->GetType() != GameObjectType("Spaceship")) return false;
    if (mBoundingShape.get() == NULL) return false;
    if (o->GetBoundingShape().get() == NULL) return false;
    return mBoundingShape->CollisionTest(o->GetBoundingShape());
}

void WeaponsPowerup::OnCollision(const GameObjectList& objects)
{
    for (auto gameObject : objects)
    {
        if (gameObject->GetType() == GameObjectType("Spaceship"))
        {
            shared_ptr<Spaceship> spaceship = static_pointer_cast<Spaceship>(gameObject);
            if (spaceship)
            {
                spaceship->UpgradeWeapon(15000);
            }
            mWorld->FlagForRemoval(GetThisPtr());
            return;
        }
    }
}