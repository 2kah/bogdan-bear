#ifndef __TowerPhysics_h_
#define __TowerPhysics_h_

#include <vector>

#include "TowerChunk.h"

class btDiscreteDynamicsWorld;
class btRigidBody;

//class Tower;
//struct BoundingVolume;

class PhysicsChunk : public TowerChunk
{
public:
    PhysicsChunk(Tower *tower, BoundingVolume bounds, btDiscreteDynamicsWorld *dynamicsWorld);
    virtual ~PhysicsChunk();

    virtual void rebuild();
protected:
    btDiscreteDynamicsWorld *dynamicsWorld;
    btRigidBody *body;
};

class TowerPhysics
{
public:
    TowerPhysics(Tower *tower, btDiscreteDynamicsWorld *dynamicsWorld);
    virtual ~TowerPhysics();
protected:
    virtual void towerUpdated(Tower *tower, BoundingVolume bounds);

    Tower *tower;
    btDiscreteDynamicsWorld *dynamicsWorld;

    std::vector<PhysicsChunk *> chunks;
};

#endif // #ifndef __TowerPhysics_h_
