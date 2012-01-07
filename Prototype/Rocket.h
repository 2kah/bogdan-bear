#ifndef __Rocket_h_
#define __Rocket_h_

#include <boost/signal.hpp>

#include "Updatable.h"
#include "Object.h"

class Ogre::Vector3;
class Ogre::Quaternion;
class Rocket;
class Explosion;

namespace {
class RocketSignals {
public:
    boost::signal<void (Rocket *)> updated;
    boost::signal<void (Rocket *, Explosion *)> exploded;
};
}

class Rocket: public Updatable, public Object
{
public:
    static const double SPEED;

    Rocket(Ogre::Vector3 position, Ogre::Quaternion orientation);
    virtual ~Rocket();

    virtual void update();

    RocketSignals signals;

    virtual void explode();
private:
    unsigned timer;
};

#endif // #ifndef __Rocket_h_
