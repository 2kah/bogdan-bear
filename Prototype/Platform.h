#ifndef __Platform_h_
#define __Platform_h_

#include <boost/signals.hpp>

#include <OGRE/OgreVector3.h>
#include <OGRE/OgreQuaternion.h>

#include "Updatable.h"
#include "Object.h"

class Platform;

namespace {
struct PlatformSignals {
    boost::signal<void (Platform *)> updated;
    boost::signal<void (Platform *)> expired;
    boost::signal<void (Platform *)> destroyed;
};
}

class Platform: public Updatable, public Object
{
public:
    Platform(Ogre::Vector3 position, Ogre::Quaternion orientaion);
    virtual ~Platform();

    virtual void update();
    virtual void destroy();

    PlatformSignals signals;
protected:
    unsigned timer;
};

#endif // #ifndef __Platform_h_
