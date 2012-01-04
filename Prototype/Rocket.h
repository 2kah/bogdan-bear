#ifndef __Rocket_h_
#define __Rocket_h_

#include <boost/signal.hpp>

#include "Updatable.h"
#include "Object.h"

class Ogre::Vector3;
class Rocket;
class Explosion;

namespace {
class RocketSignals {
public:
    boost::signal<void (Rocket *, Explosion *)> exploded;
};
}

class Rocket: public Updatable, public Object
{
public:
    Rocket(Ogre::Vector3 position);
    virtual ~Rocket();

    virtual void update();

    RocketSignals signals;
private:
    unsigned timer;
};

#endif // #ifndef __Rocket_h_
