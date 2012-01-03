#ifndef __Object_h_
#define __Object_h_

//#include <boost/signals2.hpp>

#include <OgreVector3.h>

class Object
{
public:
    virtual ~Object(void) {};
//protected:
    Ogre::Vector3 position;
    Ogre::Vector3 orientation;
};

#endif // #ifndef __Object_h_
