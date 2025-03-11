#ifndef __MOVEMENTCONTROLLER_H__
#define __MOVEMENTCONTROLLER_H__

//#include "SmartPtr.h"
//
//using namespace idllib;

class GameObject;

class MovementController
{
public:
	MovementController( shared_ptr<GameObject> object );
	virtual ~MovementController(void);

	void Accelerate(float a);
	void Rotate(float r);

protected:
	shared_ptr<GameObject> mObject;
	float mAcceleration;
};

#endif