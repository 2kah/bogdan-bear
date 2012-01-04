#ifndef __Object_h_
#define __Object_h_

#include <OGRE/OgreVector3.h>
#include <OGRE/OgreQuaternion.h>

class Object
{
public:
    virtual ~Object(void) {};
//protected:
    Ogre::Vector3 position;
    Ogre::Quaternion orientation;
};

#endif // #ifndef __Object_h_
