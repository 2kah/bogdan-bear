#ifndef __PhysicsObject_h_
#define __PhysicsObject_h_

enum PHYSICS_OBJECT_TYPE {
    UNDEFINED,
    TOWER_CHUNK,
    PLATFORM
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

    PHYSICS_OBJECT_TYPE type;
};

#endif // #ifndef __PhysicsObject_h_
