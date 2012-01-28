#ifndef __RocketPhysics_h_
#define __RocketPhysics_h_

class btDiscreteDynamicsWorld;
class btPairCachingGhostObject;

class Rocket;
class Explosion;

class RocketPhysics
{
public:
    RocketPhysics(Rocket *rocket, btDiscreteDynamicsWorld *dynamicsWorld);
    virtual ~RocketPhysics();
protected:
    virtual void rocketUpdated(Rocket *rocket);
    virtual void rocketExploded(Rocket *rocket, Explosion *explosion);

    Rocket *rocket;
    btDiscreteDynamicsWorld *dynamicsWorld;
    
    btPairCachingGhostObject *ghost;
};

#endif // #ifndef __RocketPhysics_h_
