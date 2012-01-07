#ifndef __RocketPhysics_h_
#define __RocketPhysics_h_

class btDiscreteDynamicsWorld;
class btGhostObject;

class Rocket;
class Explosion;

class RocketPhysics
{
public:
    RocketPhysics(Rocket *explosion, btDiscreteDynamicsWorld *dynamicsWorld);
    virtual ~RocketPhysics();
protected:
    virtual void rocketUpdated(Rocket *rocket);
    virtual void rocketExploded(Rocket *rocket, Explosion *explosion);

    Rocket *explosion;
    btDiscreteDynamicsWorld *dynamicsWorld;
    
    btGhostObject *ghost;
};

#endif // #ifndef __RocketPhysics_h_
