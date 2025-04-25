#include "ExtraLifePowerup.h"
#include "BoundingSphere.h"
#include "GameUtil.h"
#include "GameWorld.h"

ExtraLifePowerup::ExtraLifePowerup(void) : GameObject("ExtraLifePowerup")
{
    mType = GameObjectType("ExtraLifePowerup");
    mPosition.x = 0.0f;
    mPosition.y = 0.0f;
    mPosition.z = 0.0f;
    mTimeToLive = 20000;
}

ExtraLifePowerup::ExtraLifePowerup(const GLVector3f& position) : GameObject("ExtraLifePowerup")
{
    mPosition = position;
    mTimeToLive = 20000;
}

ExtraLifePowerup::~ExtraLifePowerup(void)
{
}

void ExtraLifePowerup::Update(int t)
{
    GameObject::Update(t);
    mTimeToLive -= t;
    if (mTimeToLive <= 0)
    {
        //if (mWorld) mWorld->FlagForRemoval(GetThisPtr());
        return;
    }
}

bool ExtraLifePowerup::CollisionTest(shared_ptr<GameObject> o)
{
    if (o->GetType() != GameObjectType("Spaceship")) return false;
    if (mBoundingShape.get() == NULL) return false;
    if (o->GetBoundingShape().get() == NULL) return false;
    return mBoundingShape->CollisionTest(o->GetBoundingShape());
}

void ExtraLifePowerup::OnCollision(const GameObjectList& objects)
{
    for (auto gameObject : objects)
    {
        if (gameObject->GetType() == GameObjectType("Spaceship"))
        {
            mWorld->FlagForRemoval(GetThisPtr());
            return;
        }
    }
}