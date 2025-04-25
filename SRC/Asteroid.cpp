#include <stdlib.h>
#include "GameUtil.h"
#include "Asteroid.h"
#include "SmallAsteroid.h"
#include "BoundingShape.h"
#include "BoundingSphere.h"
#include "Spaceship.h"

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
    if (o->GetType() == GameObjectType("ExtraLifePowerup")) return false;
    if (mBoundingShape.get() == NULL) return false;
    if (o->GetBoundingShape().get() == NULL) return false;
    return mBoundingShape->CollisionTest(o->GetBoundingShape());
}

void Asteroid::OnCollision(const GameObjectList& objects)
{
    for (auto gameObject : objects)
    {
        // If collision with another asteroid, bounce
        if (gameObject->GetType() == GameObjectType("Asteroid") ||
            gameObject->GetType() == GameObjectType("SmallAsteroid"))
        {
            shared_ptr<Asteroid> otherAsteroid = static_pointer_cast<Asteroid>(gameObject);
            Bounce(otherAsteroid);
            return;
        }

        // Check for bullet collision
        if (gameObject->GetType() == GameObjectType("Bullet"))
        {
            mDestroyedByBullet = true;
            // Create 2-3 small asteroids
            int numSmallAsteroids = 2 + (rand() % 2);

            for (int i = 0; i < numSmallAsteroids; i++)
            {
                // Small asteroid new velocities
                float angle = ((float)rand() / RAND_MAX) * 360.0f;
                GLVector3f newVelocity = mVelocity * 0.5f;
                GLVector3f variation(cos(DEG2RAD * angle), sin(DEG2RAD * angle), 0.0f);
                newVelocity = newVelocity + variation * 5.0f;

                // Small asteroid new position
                GLVector3f offset(
                    ((float)rand() / RAND_MAX * 2.0f - 1.0f) * 2.0f,
                    ((float)rand() / RAND_MAX * 2.0f - 1.0f) * 2.0f,
                    0.0f
                );
                GLVector3f newPosition = mPosition + offset;

                // Create small asteroid
                shared_ptr<SmallAsteroid> smallAsteroid = make_shared<SmallAsteroid>(newPosition, newVelocity);

                // Sprite and bounding shape
                smallAsteroid->SetSprite(mSprite);
                shared_ptr<BoundingSphere> boundingSphere = make_shared<BoundingSphere>(smallAsteroid, 5.0f);
                smallAsteroid->SetBoundingShape(boundingSphere);

                // Add to world
                mWorld->AddObject(smallAsteroid);
            }

            // Remove this asteroid
            mWorld->FlagForRemoval(GetThisPtr());
            return;
        }

        // Spaceship collision
        if (gameObject->GetType() == GameObjectType("Spaceship"))
        {
            // Check if spaceship is invincible
            shared_ptr<Spaceship> spaceship = static_pointer_cast<Spaceship>(gameObject);
            if (spaceship->IsInvincible())
            {
                // Bounce off invincible spaceship
                GLVector3f shipPosition = gameObject->GetPosition();

                // Direction from ship to asteroid
                GLVector3f bounceDirection = mPosition - shipPosition;
                float distance = sqrt(bounceDirection.x * bounceDirection.x +
                    bounceDirection.y * bounceDirection.y);

                if (distance < 0.0001f) return;

                // Normalize direction
                bounceDirection.x /= distance;
                bounceDirection.y /= distance;

                // Reverse velocity with some randomness
                mVelocity = -mVelocity + GLVector3f(
                    ((float)rand() / RAND_MAX * 2.0f - 1.0f) * 2.0f,
                    ((float)rand() / RAND_MAX * 2.0f - 1.0f) * 2.0f,
                    0.0f
                );

                // Move away from ship to prevent sticking
                mPosition = mPosition + bounceDirection * 1.0f;

                return;
            }
            mDestroyedByBullet = false;
            mWorld->FlagForRemoval(GetThisPtr());
            return;
        }
    }
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