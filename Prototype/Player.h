#ifndef __Player_h_
#define __Player_h_

#include <boost/signal.hpp>

#include "Updatable.h"
#include "Object.h"

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
    boost::signal<void (Player *)> updated;

    boost::signal<void (Player *, Rocket *)> fired;
    boost::signal<void (Player *, Platform *)> platform;

    boost::signal<void (Player *, Turret *)> enteredTurret;
    boost::signal<void (Player *, Turret *)> exitedTurret;
};
}

class Player : public Updatable, public Object
{
public:
    static const double ROTATION_SPEED;
    static const double MOVEMENT_SPEED;

    Player(Ogre::Vector3 position);
    virtual ~Player();

    virtual void update();

    PlayerSignals signals;

    virtual void addInput(PlayerInput &input);

    virtual void movement(DIRECTION direction, bool state);
    virtual void look(int x, int y);
    
    virtual void fire(bool state);
    virtual void create(bool state);

    virtual void jump(bool state);
    virtual void use(bool state);

    Ogre::Quaternion relativeAim;

    Turret *turret;
private:
    Ogre::Vector3 velocity;

    Ogre::Radian rotX;
    Ogre::Radian rotY;
};

#endif // #ifndef __Player_h_
