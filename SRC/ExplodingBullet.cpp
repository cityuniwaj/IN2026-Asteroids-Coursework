#include "ExplodingBullet.h"
#include "BoundingSphere.h"
#include "GameWorld.h"
#include "Asteroid.h"
#include "SmallAsteroid.h"
#include "Explosion.h"
#include "Animation.h"
#include "AnimationManager.h"
#include "Sprite.h"

ExplodingBullet::ExplodingBullet()
    : GameObject("ExplodingBullet"), mTimeToLive(3000), mExploded(false) // 3 second lifetime
{
}

ExplodingBullet::ExplodingBullet(GLVector3f p, GLVector3f v, GLfloat h, GLfloat ttl)
    : GameObject("ExplodingBullet"), mExploded(false)
{
    mPosition = p;
    mVelocity = v;
    mAngle = h;
    mTimeToLive = 3000;
}

ExplodingBullet::~ExplodingBullet(void)
{
}

void ExplodingBullet::Update(int t)
{
    GameObject::Update(t);
    mTimeToLive -= t;

    if (mTimeToLive <= 0 && !mExploded)
    {
        Explode();
    }
}

bool ExplodingBullet::CollisionTest(shared_ptr<GameObject> o)
{
    if (o->GetType() == GameObjectType("Asteroid") ||
        o->GetType() == GameObjectType("SmallAsteroid"))
    {
        if (mBoundingShape.get() == NULL) return false;
        if (o->GetBoundingShape().get() == NULL) return false;
        return mBoundingShape->CollisionTest(o->GetBoundingShape());
    }
    return false;
}

void ExplodingBullet::OnCollision(const GameObjectList& objects)
{
    if (!mExploded)
    {
        for (auto obj : objects)
        {
            if (obj->GetType() == GameObjectType("Asteroid") ||
                obj->GetType() == GameObjectType("SmallAsteroid"))
            {
                if (mWorld) mWorld->FlagForRemoval(obj);
            }
        }
        Explode();
    }
}

void ExplodingBullet::Explode()
{
    mExploded = true;

    CreateExplosionEffect();

    for (int i = 0; i < 6; i++) {
        float angle = i * 60.0f * M_PI / 180.0f;
        GLVector3f offset(10.0f * sinf(angle), 10.0f * cosf(angle), 0.0f);

        Animation* anim_ptr = AnimationManager::GetInstance().GetAnimationByName("explosion");
        if (anim_ptr) {
            shared_ptr<Sprite> explosion_sprite = make_shared<Sprite>(anim_ptr->GetWidth(), anim_ptr->GetHeight(), anim_ptr);
            explosion_sprite->SetLoopAnimation(false);

            shared_ptr<GameObject> explosion = make_shared<Explosion>();
            explosion->SetSprite(explosion_sprite);
            explosion->SetPosition(mPosition + offset);
            explosion->SetScale(0.3f);

            if (mWorld) mWorld->AddObject(explosion);
        }
    }
    if (mWorld) mWorld->FlagForRemoval(GetThisPtr());
}

void ExplodingBullet::CreateExplosionEffect()
{
    Animation* anim_ptr = AnimationManager::GetInstance().GetAnimationByName("explosion");
    if (anim_ptr)
    {
        shared_ptr<Sprite> explosion_sprite = make_shared<Sprite>(
            anim_ptr->GetWidth(), anim_ptr->GetHeight(), anim_ptr);
        explosion_sprite->SetLoopAnimation(false);

        shared_ptr<GameObject> explosion = make_shared<Explosion>();
        explosion->SetSprite(explosion_sprite);
        explosion->SetPosition(mPosition);
        explosion->SetScale(0.5f);

        if (mWorld) mWorld->AddObject(explosion);
    }
}