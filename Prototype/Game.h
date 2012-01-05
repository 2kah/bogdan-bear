#ifndef __Game_h_
#define __Game_h_

#include <vector>
#include <set>

#include <btBulletDynamicsCommon.h>
class TowerOld;

#include "BaseApplication.h"

class Updatable;
class Tower;
class Player;
class Rocket;
class Explosion;

class SceneObject;
class FallingObject;
class GameTestThing;

class Game : public BaseApplication
{
public:
    Game(void);
    virtual ~Game(void);

    virtual void run(void);

    virtual void carveSphere(Ogre::Vector3 position, double radius);

protected:
    virtual void createScene(void);

    virtual bool keyPressed(const OIS::KeyEvent &arg);
    virtual bool keyReleased(const OIS::KeyEvent &arg);
	virtual bool mouseMoved(const OIS::MouseEvent &arg);
    virtual bool mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
    virtual bool mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id);

    virtual void createCamera(void);
    virtual void createViewports(void);

private:
    Player *player;
    FallingObject *fallingObject;
    btDiscreteDynamicsWorld* dynamicsWorld;

    std::set<Updatable *> objects;

#ifdef __USE_OLD_TOWER__
    TowerOld *tower;
#else
    Tower* tower;
#endif  

    Ogre::Camera* mCamera;

    GameTestThing *gameTestThing;

    friend class GameTestThing;
};

#endif // #ifndef __Game_h_
