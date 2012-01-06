#ifndef __TowerPhysics_h_
#define __TowerPhysics_h_

class btDiscreteDynamicsWorld;

class Tower;

class TowerPhysics
{
public:
    TowerPhysics(Tower *tower, btDiscreteDynamicsWorld *dynamicsWorld);
    virtual ~TowerPhysics();
protected:
    Tower *tower;
};

#endif // #ifndef __TowerPhysics_h_
