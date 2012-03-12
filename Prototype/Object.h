#ifndef __Object_h_
#define __Object_h_

#include <boost/signals.hpp>

#include <OGRE/OgreVector3.h>
#include <OGRE/OgreQuaternion.h>

class Object : public boost::signals::trackable
{
public:
    Object()
        : position()
        , orientation()
    {}
    Object(Ogre::Vector3 position, Ogre::Quaternion orientation)
        : position(position)
        , orientation(orientation)
    {}
    virtual ~Object() {};
//protected:
    Ogre::Vector3 position;
    Ogre::Quaternion orientation;
};

#endif // #ifndef __Object_h_
