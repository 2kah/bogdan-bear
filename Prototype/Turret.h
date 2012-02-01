#ifndef __Turret_h_
#define __Turret_h_

#include <boost/signals.hpp>

#include <OGRE/OgreVector3.h>
#include <OGRE/OgreQuaternion.h>

#include "Updatable.h"
#include "Object.h"

class Turret;
class Rocket;

namespace {
struct TurretSignals {
    boost::signal<void (Turret *)> updated;
    boost::signal<void (Turret *, Rocket *)> fired;
};
}

class Turret: public Updatable, public Object
{
public:
    Turret(Ogre::Vector3 position, Ogre::Quaternion orientaion);
    virtual ~Turret();

    virtual void update();

    TurretSignals signals;

    virtual void setTarget(Object *target);

	virtual bool isOccupied();
protected:
    unsigned timer;
	Ogre::Vector3 oldtarget;
	Ogre::Vector3 nptarget;
	Ogre::Vector3 currenttarget;
    Object *target;
	bool occ;
};

#endif // #ifndef __Turret_h_
