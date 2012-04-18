#ifndef __TowerPhysics_h_
#define __TowerPhysics_h_

#include <vector>

#include <boost/signal.hpp>

#include "PhysicsObject.h"
#include "TowerChunk.h"

class btTriangleMesh;
class btDiscreteDynamicsWorld;
class btRigidBody;

//class Tower;
//struct BoundingVolume;

class PhysicsChunk : public TowerChunk, public PhysicsObject
{
public:
    PhysicsChunk(Tower *tower, BoundingVolume bounds, btDiscreteDynamicsWorld *dynamicsWorld);
    virtual ~PhysicsChunk();

    virtual void rebuild();
protected:
    btTriangleMesh *mesh;
    btDiscreteDynamicsWorld *dynamicsWorld;
    btRigidBody *body;
};

class TowerPhysics : public boost::signals::trackable
{
public:
    TowerPhysics(Tower *tower, btDiscreteDynamicsWorld *dynamicsWorld);
    virtual ~TowerPhysics();
protected:
    virtual void towerUpdated(Tower *tower, BoundingVolume bounds);
    virtual void towerRemoved(Tower *tower);

    Tower *tower;
    btDiscreteDynamicsWorld *dynamicsWorld;

    std::vector<PhysicsChunk *> chunks;
};

#endif // #ifndef __TowerPhysics_h_
