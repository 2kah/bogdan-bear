#ifndef __Explosion_h_
#define __Explosion_h_

#include <boost/signals.hpp>

#include <OGRE/OgreVector3.h>

#include "Updatable.h"
#include "Object.h"

class Explosion;

namespace {
struct ExplosionSignals {
    boost::signal<void (Explosion *)> updated;
    boost::signal<void (Explosion *)> finished;
};
}

class Explosion: public Updatable, public Object
{
public:
    static const double SIZE;

    Explosion(Ogre::Vector3 position);
	Explosion(Ogre::Vector3 position, bool isMassive);
    virtual ~Explosion();

    virtual void update();

    ExplosionSignals signals;
//protected:
    int size;
	bool isMassive;
};

#endif // #ifndef __Explosion_h_
