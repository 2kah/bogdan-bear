#ifndef __ExplosionPhysics_h_
#define __ExplosionPhysics_h_

class btDiscreteDynamicsWorld;
class btPairCachingGhostObject;

class Explosion;

class Tower;

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
    
    btPairCachingGhostObject *ghost;

    Tower *tower;
};

#endif // #ifndef __ExplosionPhysics_h_
