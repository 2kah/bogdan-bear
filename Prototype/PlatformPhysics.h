#ifndef __PlatformPhysics_h_
#define __PlatformPhysics_h_

#include "PhysicsObject.h"

class btDiscreteDynamicsWorld;
class btRigidBody;

class Platform;

class PlatformPhysics : public PhysicsObject
{
public:
    PlatformPhysics(Platform *platform, btDiscreteDynamicsWorld *dynamicsWorld);
    virtual ~PlatformPhysics();

    Platform *platform;
protected:
    virtual void platformExpired(Platform *platform);

    btDiscreteDynamicsWorld *dynamicsWorld;    
	btRigidBody *body;
};

#endif // #ifndef __PlatformPhysics_h_
