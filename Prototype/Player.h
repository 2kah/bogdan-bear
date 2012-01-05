#ifndef __Player_h_
#define __Player_h_

#include <boost/signal.hpp>

#include "Updatable.h"
#include "Object.h"

class btDiscreteDynamicsWorld;

class Player;
class Platform;
class Rocket;

namespace {
struct PlayerSignals {
    boost::signal<void (Player *)> updated;
    boost::signal<void (Player *, Rocket *)> fired;
    boost::signal<void (Player *, Platform *)> platform;
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

    virtual void forward();
    virtual void back();
    virtual void left();
    virtual void right();

    virtual void stopMovingForward();
    virtual void stopMovingBack();
    virtual void stopMovingLeft();
    virtual void stopMovingRight();

    virtual void jump();
    virtual void shoot(Ogre::SceneManager *mSceneMgr, btDiscreteDynamicsWorld* dynamicsWorld);
    virtual void platform();

    virtual void lookX(int dist);
    virtual void lookY(int dist);

    Ogre::Quaternion relativeAim;
private:
    Ogre::Vector3 velocity;

    Ogre::Radian rotX;
    Ogre::Radian rotY;
};

#endif // #ifndef __Player_h_
