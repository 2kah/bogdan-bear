#ifndef __Player_h_
#define __Player_h_

#include <boost/signal.hpp>

#include "Updatable.h"
#include "Object.h"
#include "Turret.h"
#include "PlayerProperties.h"

class btDiscreteDynamicsWorld;
class TowerOld;

class PlayerInput;
enum DIRECTION;

class Turret;
class Player;
class Platform;
class Rocket;

namespace {
struct PlayerSignals {
    boost::signal<void (Player *)> removed;

    boost::signal<void (Player *)> updated;
    boost::signal<void (Player *)> stateSet;

    boost::signal<void (Player *, Rocket *)> fired;
    boost::signal<void (Player *, Platform *)> platform;
	boost::signal<void (Ogre::Vector3 position, Ogre::Quaternion orientation)> sendPlatform;

    boost::signal<void (Player *)> used;

    boost::signal<void (Player *)> startedStepping;
    boost::signal<void (Player *)> stoppedStepping;

    // TODO: remove these
    boost::signal<void (Player * /*, Turret **/)> enteredTurret;
    boost::signal<void (Player * /*, Turret **/)> exitedTurret;
};
}

class Player : public Updatable, public Object
{
public:
    static const double ROTATION_SPEED;
    static const double MOVEMENT_SPEED;

    Player(Ogre::Vector3 position);
    virtual ~Player();

    virtual void setState(Ogre::Vector3 position,
                          Ogre::Vector3 velocity,
                          Ogre::Quaternion orientation);

    virtual void update();

    PlayerSignals signals;

    virtual void addInput(PlayerInput &input);

    virtual void movement(DIRECTION direction, bool state);
    virtual void look(int x, int y);
    
    virtual void fire(bool state);
    virtual void create(bool state);

    virtual void jump(bool state);
    virtual void use(bool state);

	//TODO: remove these 2 (testing purposes only)
	virtual void enteredTurret();
	virtual void exitedTurret();

	virtual void setTurret(Turret *newTurret);

	virtual double getAmmo();
	virtual int getAmmoAmount();

	virtual void setScores(int newScores[]);
	virtual int getScore(int team);

	virtual double getPlatformReload();

    Ogre::Quaternion relativeAim;
    Ogre::Vector3 velocity;

	PlayerProperties* prop;

	bool anim;

    bool stepping;

    int spawnID;

	bool isInTurret;

private:
    Ogre::Radian rotX;
    Ogre::Radian rotY;

	int rocketAmmo;
	bool needsReload;
	int reloadTimer;
	bool canFire;
	int cooldown;
	int scores[4];
	int platformTimer;

	Ogre::Vector3 oldPosition;
	Ogre::Vector3 newPosition;

	Turret *turret;

};

#endif // #ifndef __Player_h_
