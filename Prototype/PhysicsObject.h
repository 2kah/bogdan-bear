#ifndef __PhysicsObject_h_
#define __PhysicsObject_h_

class Explosion;

enum PHYSICS_OBJECT_TYPE {
    UNDEFINED,
    TOWER_CHUNK,
    PLATFORM,
	PLAYER,
    ROCKET
};

class PhysicsObject
{
public:
    PhysicsObject()
        : type(UNDEFINED)
    {}
    PhysicsObject(PHYSICS_OBJECT_TYPE type)
        : type(type)
    {}
    virtual ~PhysicsObject() {}

    virtual void explode(Explosion *explosion) {};

    PHYSICS_OBJECT_TYPE type;
};

#endif // #ifndef __PhysicsObject_h_
