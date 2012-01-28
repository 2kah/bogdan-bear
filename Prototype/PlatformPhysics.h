#ifndef __PlatformPhysics_h_
#define __PlatformPhysics_h_

class btDiscreteDynamicsWorld;
class btRigidBody;

class Platform;

class PlatformPhysics
{
public:
    PlatformPhysics(Platform *platform, btDiscreteDynamicsWorld *dynamicsWorld);
    virtual ~PlatformPhysics();
protected:
    //virtual void platformUpdated(Platform *platform);
    virtual void platformExpired(Platform *platform);

    Platform *platform;
    btDiscreteDynamicsWorld *dynamicsWorld;
    
	btRigidBody *body;
};

#endif // #ifndef __PlatformPhysics_h_
