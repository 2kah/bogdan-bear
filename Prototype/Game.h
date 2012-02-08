#ifndef __Game_h_
#define __Game_h_

#include <vector>
#include <set>

#include <btBulletDynamicsCommon.h>
#include "BtOgreExtras.h"

#include "BaseApplication.h"
#include "PlayerInput.h"

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
    Game();
    virtual ~Game(void);

    virtual void run(void);
protected:
    virtual bool keyPressed(const OIS::KeyEvent &arg);
    virtual bool keyReleased(const OIS::KeyEvent &arg);
	virtual bool mouseMoved(const OIS::MouseEvent &arg);
    virtual bool mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
    virtual bool mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id);

    btDiscreteDynamicsWorld* dynamicsWorld;
    BtOgre::DebugDrawer *mDebugDrawer;

    Tower *tower;
    GameTestThing *gameTestThing;

    PlayerInput playerInput;
private:
    std::set<Updatable *> objects;
    Ogre::Camera* mCamera;

    friend class GameTestThing;
};

#endif // #ifndef __Game_h_
