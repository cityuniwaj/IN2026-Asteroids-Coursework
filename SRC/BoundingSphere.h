#ifndef __BOUNDINGSPHERE_H__
#define __BOUNDINGSPHERE_H__

#include "GameUtil.h"
#include "GameObjectType.h"
#include "BoundingShape.h"

class BoundingSphere : public BoundingShape
{
public:
	BoundingSphere() : BoundingShape("BoundingSphere"), mRadius(0) {}
	BoundingSphere(shared_ptr<GameObject> o, float r)
		: BoundingShape("BoundingSphere", o), mRadius(r) {}

	bool CollisionTest(shared_ptr<BoundingShape> bs) {
		if (GetType() == bs->GetType()) {
			BoundingSphere* bsphere = (BoundingSphere*)bs.get();
			GLVector3f pos1 = GetGameObject()->GetPosition();
			GLVector3f pos2 = bsphere->GetGameObject()->GetPosition();
			float distanceSqr = (pos2 - pos1).lengthSqr();
			float collision_distance = GetRadius() + bsphere->GetRadius();
			return (distanceSqr <= pow(collision_distance, 2));
		}
		return false;
	}

	void SetRadius(float r) { mRadius = r; }
	float GetRadius() { return mRadius; }

protected:
	float mRadius;
};

#endif
