#ifndef __ExplosionPhysics_h_
#define __ExplosionPhysics_h_

class btDiscreteDynamicsWorld;
class btGhostObject;

class Explosion;

class ExplosionPhysics
{
public:
    ExplosionPhysics(Explosion *explosion, btDiscreteDynamicsWorld *dynamicsWorld);
    virtual ~ExplosionPhysics();
protected:
    virtual void explosionUpdated(Explosion *explosion);
    virtual void explosionFinished(Explosion *explosion);

    Explosion *explosion;
    btDiscreteDynamicsWorld *dynamicsWorld;
    
    btGhostObject *ghost;
};

#endif // #ifndef __ExplosionPhysics_h_
