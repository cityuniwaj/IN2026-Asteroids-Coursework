#include "GameUtil.h"
#include "GameWorld.h"
#include "Bullet.h"
#include "ExplodingBullet.h"
#include "Spaceship.h"
#include "BoundingSphere.h"
#include "Animation.h"
#include "AnimationManager.h"

using namespace std;

// PUBLIC INSTANCE CONSTRUCTORS ///////////////////////////////////////////////

/**  Default constructor. */
Spaceship::Spaceship()
	: GameObject("Spaceship"), mThrust(0), mIsInvincible(false), mInvincibilityTime(0), mWeaponUpgraded(false), mWeaponUpgradeTimeLeft(0)
{
}

/** Construct a spaceship with given position, velocity, acceleration, angle, and rotation. */
Spaceship::Spaceship(GLVector3f p, GLVector3f v, GLVector3f a, GLfloat h, GLfloat r)
	: GameObject("Spaceship", p, v, a, h, r), mThrust(0)
{
}

/** Copy constructor. */
Spaceship::Spaceship(const Spaceship& s)
	: GameObject(s), mThrust(0)
{
}

/** Destructor. */
Spaceship::~Spaceship(void)
{
}

// PUBLIC INSTANCE METHODS ////////////////////////////////////////////////////

/** Update this spaceship. */
void Spaceship::Update(int t)
{
	// Call parent update function
	GameObject::Update(t);

	if (mIsInvincible)
	{
		mInvincibilityTime -= t;
		if (mInvincibilityTime <= 0)
		{
			mIsInvincible = false;
			if (mNormalSprite)
			{
				SetSprite(mNormalSprite);
			}
		}
	}

	if (mWeaponUpgraded)
	{
		mWeaponUpgradeTimeLeft -= t;
		if (mWeaponUpgradeTimeLeft <= 0)
		{
			mWeaponUpgraded = false;
		}
	}
}

/** Render this spaceship. */
void Spaceship::Render(void)
{
	if (mSpaceshipShape.get() != NULL) mSpaceshipShape->Render();

	// If ship is thrusting
	if ((mThrust > 0) && (mThrusterShape.get() != NULL)) {
		mThrusterShape->Render();
	}

	GameObject::Render();
}

/** Fire the rockets. */
void Spaceship::Thrust(float t)
{
	mThrust = t*5;
	// Increase acceleration in the direction of ship
	mAcceleration.x = mThrust*cos(DEG2RAD*mAngle);
	mAcceleration.y = mThrust*sin(DEG2RAD*mAngle);
}

/** Set the rotation. */
void Spaceship::Rotate(float r)
{
	mRotation = r*2.5;
}

/** Shoot a bullet. */
void Spaceship::Shoot(void)
{
	if (mWeaponUpgraded)
	{
		ShootUpgradedWeapon();
	}
	else
	{
		// Check the world exists
		if (!mWorld) return;
		// Construct a unit length vector in the direction the spaceship is headed
		GLVector3f spaceship_heading(cos(DEG2RAD * mAngle), sin(DEG2RAD * mAngle), 0);
		spaceship_heading.normalize();
		// Calculate the point at the node of the spaceship from position and heading
		GLVector3f bullet_position = mPosition + (spaceship_heading * 4);
		// Calculate how fast the bullet should travel
		float bullet_speed = 30;
		// Construct a vector for the bullet's velocity
		GLVector3f bullet_velocity = mVelocity + spaceship_heading * bullet_speed;
		// Construct a new bullet
		shared_ptr<GameObject> bullet
		(new Bullet(bullet_position, bullet_velocity, mAcceleration, mAngle, 0, 2000));
		bullet->SetBoundingShape(make_shared<BoundingSphere>(bullet->GetThisPtr(), 2.0f));
		bullet->SetShape(mBulletShape);
		// Add the new bullet to the game world
		mWorld->AddObject(bullet);
	}
}

bool Spaceship::CollisionTest(shared_ptr<GameObject> o)
{
	if (o->GetType() == GameObjectType("ExtraLifePowerup") ||
		o->GetType() == GameObjectType("InvincibilityPowerup"))
	{
		if (mBoundingShape.get() == NULL) return false;
		if (o->GetBoundingShape().get() == NULL) return false;
		return mBoundingShape->CollisionTest(o->GetBoundingShape());
	}
	
	if (mIsInvincible && o->GetType() == GameObjectType("Asteroid")){return false;}

	if (o->GetType() != GameObjectType("Asteroid")) return false;
	if (mBoundingShape.get() == NULL) return false;
	if (o->GetBoundingShape().get() == NULL) return false;
	return mBoundingShape->CollisionTest(o->GetBoundingShape());
}

void Spaceship::OnCollision(const GameObjectList& objects)
{
	if (mIsInvincible) {return;}

	for (auto gameObject : objects)
	{
		if (gameObject->GetType() == GameObjectType("SmallAsteroid"))
		{
			return;
		}

		if (gameObject->GetType() == GameObjectType("Asteroid"))
		{
			mWorld->FlagForRemoval(GetThisPtr());
			return;
		}
	}
}

void Spaceship::MakeInvincible(int duration)
{
	mIsInvincible = true;
	mInvincibilityTime = duration;
	if (!mNormalSprite)
	{
		mNormalSprite = mSprite;
	}

	Animation* anim_ptr = AnimationManager::GetInstance().GetAnimationByName("shipBubble");
	if (anim_ptr)
	{
		shared_ptr<Sprite> invincibleSprite = make_shared<Sprite>(anim_ptr->GetWidth(), anim_ptr->GetHeight(), anim_ptr);
		SetSprite(invincibleSprite);
	}
}

void Spaceship::UpgradeWeapon(int duration)
{
	mWeaponUpgraded = true;
	mWeaponUpgradeTimeLeft = duration;
}

void Spaceship::ShootUpgradedWeapon()
{
    // Create a new exploding bullet using the same approach as regular bullets
    shared_ptr<GameObject> bullet = make_shared<ExplodingBullet>();
    bullet->SetBoundingShape(make_shared<BoundingSphere>(bullet->GetThisPtr(), 2.0f));
    bullet->SetScale(2.0f); // Larger bullet
    
    // Set the bullet's shape
    if (mBulletShape.get() != NULL)
    {
        bullet->SetShape(mBulletShape);
    }
    
    // Set the bullet's position to the spaceship
    GLVector3f position = mPosition;
    position.x += 4.0f * sin(mAngle * M_PI / 180.0);
    position.y -= 4.0f * cos(mAngle * M_PI / 180.0);
    bullet->SetPosition(position);
    bullet->SetAngle(mAngle);
    
    // Calculate the bullet's velocity using the same approach as the normal bullet
    GLVector3f velocity;
    velocity.x = 60.0f * sin(mAngle * M_PI / 180.0);
    velocity.y = -60.0f * cos(mAngle * M_PI / 180.0);
    velocity.z = 0.0f;
    bullet->SetVelocity(velocity);
    
    // Add the bullet to the world
    if (mWorld != NULL)
    {
        mWorld->AddObject(bullet);
    }
}