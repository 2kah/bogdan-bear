#ifndef __TowerPhysics_h_
#define __TowerPhysics_h_

class btDiscreteDynamicsWorld;

class ComplexTower;

class TowerPhysics
{
public:
    TowerPhysics(ComplexTower *tower, btDiscreteDynamicsWorld *dynamicsWorld);
    virtual ~TowerPhysics();
protected:
    ComplexTower *tower;
};

#endif // #ifndef __TowerPhysics_h_
