#include <stdlib.h>
#include "GameUtil.h"
#include "Asteroid.h"
#include "BoundingShape.h"

Asteroid::Asteroid(void) : GameObject("Asteroid")
{
    mAngle = rand() % 360;
    mRotation = 0; // rand() % 90;
    mPosition.x = rand() / 2;
    mPosition.y = rand() / 2;
    mPosition.z = 0.0;
    mVelocity.x = 10.0 * cos(DEG2RAD*mAngle);
    mVelocity.y = 10.0 * sin(DEG2RAD*mAngle);
    mVelocity.z = 0.0;
}

Asteroid::~Asteroid(void)
{
}

bool Asteroid::CollisionTest(shared_ptr<GameObject> o)
{
    if (mBoundingShape.get() == NULL) return false;
    if (o->GetBoundingShape().get() == NULL) return false;
    return mBoundingShape->CollisionTest(o->GetBoundingShape());
}

void Asteroid::OnCollision(const GameObjectList& objects)
{
    // Handle collisions with different types of objects
    for (auto gameObject : objects)
    {
        // If collision with another asteroid, bounce
        if (gameObject->GetType() == GameObjectType("Asteroid"))
        {
            shared_ptr<Asteroid> otherAsteroid = static_pointer_cast<Asteroid>(gameObject);
            Bounce(otherAsteroid);
            return;
        }
    }
    mWorld->FlagForRemoval(GetThisPtr());
}

void Asteroid::Bounce(shared_ptr<Asteroid> otherAsteroid)
{
    // Get velocities and positions
    GLVector3f myVelocity = this->GetVelocity();
    GLVector3f otherVelocity = otherAsteroid->GetVelocity();
    GLVector3f myPosition = this->GetPosition();
    GLVector3f otherPosition = otherAsteroid->GetPosition();
    GLVector3f collisionNormal = myPosition - otherPosition;
    GLVector3f relativeVelocity = myVelocity - otherVelocity;

	// Direction of the collision
    float distance = sqrt(collisionNormal.x * collisionNormal.x + collisionNormal.y * collisionNormal.y);

    collisionNormal.x /= distance;
    collisionNormal.y /= distance;


	// Relative speed in the direction of the collision normal
    float relativeSpeed = relativeVelocity.x * collisionNormal.x + relativeVelocity.y * collisionNormal.y;

    if (relativeSpeed >= 0) return;

	// Calculate mass of asteroids
    float myMass = this->GetSize();
    float otherMass = otherAsteroid->GetSize();
    float totalMass = myMass + otherMass;

    // Calculate impulse scalar
    float impulseScalar = -(2.0f * relativeSpeed) / totalMass; // Bounce energy

    // Calculate impulse vector
    GLVector3f impulse = collisionNormal * impulseScalar;

    // Apply impulse to both asteroids based on their masses
    GLVector3f myImpulse = impulse * otherMass;
    GLVector3f otherImpulse = impulse * -myMass;

    // Update velocities
    this->SetVelocity(myVelocity + myImpulse);
    otherAsteroid->SetVelocity(otherVelocity + otherImpulse);

    // Random Factor
    float randomFactor = 0.2f;
    GLVector3f myRandom(
        ((float)rand() / RAND_MAX * 2.0f - 1.0f) * randomFactor,
        ((float)rand() / RAND_MAX * 2.0f - 1.0f) * randomFactor,
        0.0f
    );

    GLVector3f otherRandom(
        ((float)rand() / RAND_MAX * 2.0f - 1.0f) * randomFactor,
        ((float)rand() / RAND_MAX * 2.0f - 1.0f) * randomFactor,
        0.0f
    );

    this->SetVelocity(this->GetVelocity() + myRandom);
    otherAsteroid->SetVelocity(otherAsteroid->GetVelocity() + otherRandom);
}