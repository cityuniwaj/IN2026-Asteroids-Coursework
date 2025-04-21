#include <stdlib.h>
#include "GameUtil.h"
#include "SmallAsteroid.h"
#include "BoundingShape.h"

SmallAsteroid::SmallAsteroid(void) : Asteroid()
{
    mType = GameObjectType("SmallAsteroid");
    mVelocity = mVelocity * 1.2f;
    mScale = 0.1f;
    mTimeToLive = 10000;
}

SmallAsteroid::SmallAsteroid(const GLVector3f& position, const GLVector3f& velocity) : Asteroid()
{
    mType = GameObjectType("SmallAsteroid");
    mPosition = position;
    mVelocity = velocity;

    float angle = ((float)rand() / RAND_MAX) * 360.0f;
    GLVector3f variation(cos(DEG2RAD * angle), sin(DEG2RAD * angle), 0.0f);
    mVelocity = mVelocity + variation * 5.0f;

    mScale = 0.1f;
    mTimeToLive = 5000;
}

SmallAsteroid::~SmallAsteroid(void)
{
}

void SmallAsteroid::OnCollision(const GameObjectList& objects)
{
    for (auto gameObject : objects)
    {
        if (gameObject->GetType() == GameObjectType("Asteroid") ||
            gameObject->GetType() == GameObjectType("SmallAsteroid"))
        {
            shared_ptr<Asteroid> otherAsteroid = static_pointer_cast<Asteroid>(gameObject);
            Bounce(otherAsteroid);
            return;
        }

        if (gameObject->GetType() == GameObjectType("Spaceship"))
        {
            GLVector3f shipPosition = gameObject->GetPosition();
            GLVector3f collisionNormal = mPosition - shipPosition;
            float distance = sqrt(collisionNormal.x * collisionNormal.x + collisionNormal.y * collisionNormal.y);

            if (distance < 0.0001f) continue;

            collisionNormal.x /= distance;
            collisionNormal.y /= distance;
            mVelocity = -mVelocity + GLVector3f(
                ((float)rand() / RAND_MAX * 2.0f - 1.0f) * 2.0f,
                ((float)rand() / RAND_MAX * 2.0f - 1.0f) * 2.0f,
                0.0f
            );
            mPosition = mPosition + collisionNormal * 1.0f;

            return;
        }
    }
    mWorld->FlagForRemoval(GetThisPtr());
}

void SmallAsteroid::Update(int t)
{
    Asteroid::Update(t);
    mTimeToLive = mTimeToLive - t;

    if (mTimeToLive < 0) 
    { 
        mTimeToLive = 0; 
    }

    if (mTimeToLive == 0) {
        if (mWorld) mWorld->FlagForRemoval(GetThisPtr());
    }
}