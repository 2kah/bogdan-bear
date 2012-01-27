#ifndef __TowerPhysics_h_
#define __TowerPhysics_h_

#include <vector>

class btDiscreteDynamicsWorld;

class Tower;
struct BoundingVolume;

class TowerPhysics
{
public:
    TowerPhysics(Tower *tower, btDiscreteDynamicsWorld *dynamicsWorld);
    virtual ~TowerPhysics();
protected:
    virtual void towerUpdated(Tower *tower, BoundingVolume bounds);

    Tower *tower;
};

#endif // #ifndef __TowerPhysics_h_
