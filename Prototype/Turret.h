#ifndef __Turret_h_
#define __Turret_h_

#include <boost/signals.hpp>

#include <OGRE/OgreVector3.h>
#include <OGRE/OgreQuaternion.h>

#include "Updatable.h"
#include "Object.h"

class Player;
class Turret;
class Rocket;
class NetworkTestStuff;
namespace {
struct TurretSignals {
    boost::signal<void (Turret *)> updated;
    boost::signal<void (Turret *, Rocket *)> fired;
};
}

class Turret: public Updatable, public Object
{
public:
	bool isClientSide;
    Turret(Ogre::Vector3 position, Ogre::Quaternion orientaion, NetworkTestStuff* net_stuff, bool client);
    virtual ~Turret();

    virtual void update();
	NetworkTestStuff* network_obj;
    TurretSignals signals;

    virtual void setTarget(Ogre::Vector3 positionTarget);

	virtual bool setOccupied(bool set, Player *player);

	virtual bool isOccupied();

	virtual void fireTurret();

	virtual void setOccupant(Ogre::Quaternion newOO, Ogre::Quaternion newRA);

protected:
	bool occ;
    unsigned timer;
	Ogre::Vector3 oldtarget;
	Ogre::Vector3 nptarget;
	Ogre::Vector3 currenttarget;
	Ogre::Quaternion occupantOrientation;
	Ogre::Quaternion occupantRelativeAim;
	
	int rockets;
	Player *player;
	unsigned playerTimer;
};

#endif // #ifndef __Turret_h_
