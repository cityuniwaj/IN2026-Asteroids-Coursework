#include "InvincibilityPowerup.h"
#include "BoundingSphere.h"
#include "GameUtil.h"
#include "GameWorld.h"
#include "Spaceship.h"

InvincibilityPowerup::InvincibilityPowerup(void) : GameObject("InvincibilityPowerup")
{
    mPosition.x = 0.0f;
    mPosition.y = 0.0f;
    mPosition.z = 0.0f;
    mTimeToLive = 20000;
}

InvincibilityPowerup::InvincibilityPowerup(const GLVector3f& position) : GameObject("InvincibilityPowerup")
{
    mPosition = position;
    mAngle = 0;
    mRotation = 0;
    mTimeToLive = 20000;
}

InvincibilityPowerup::~InvincibilityPowerup(void)
{
}

void InvincibilityPowerup::Update(int t)
{
    GameObject::Update(t);
    mTimeToLive -= t;
    if (mTimeToLive <= 0)
    {
        //if (mWorld) mWorld->FlagForRemoval(GetThisPtr());
        return;
    }
}

bool InvincibilityPowerup::CollisionTest(shared_ptr<GameObject> o)
{
    if (o->GetType() != GameObjectType("Spaceship")) return false;
    if (mBoundingShape.get() == NULL) return false;
    if (o->GetBoundingShape().get() == NULL) return false;
    return mBoundingShape->CollisionTest(o->GetBoundingShape());
}

void InvincibilityPowerup::OnCollision(const GameObjectList& objects)
{
    for (auto gameObject : objects)
    {
        if (gameObject->GetType() == GameObjectType("Spaceship"))
        {
            shared_ptr<Spaceship> spaceship = static_pointer_cast<Spaceship>(gameObject);
            if (spaceship)
            {
                spaceship->MakeInvincible(10000);
            }
            mWorld->FlagForRemoval(GetThisPtr());
            return;
        }
    }
}