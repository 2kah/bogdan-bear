#ifndef __Game_h_
#define __Game_h_

#include <vector>
#include <set>

#include <OGRE/OgreOverlay.h>
#include <OGRE/OgreOverlayManager.h>

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
	bool game;
    Game();
    virtual ~Game(void);

    virtual void run(char *hostIP);

	void setUpPhysicsWorld(void);
	void restartClient();
protected:
    virtual bool keyPressed(const OIS::KeyEvent &arg);
    virtual bool keyReleased(const OIS::KeyEvent &arg);
	virtual bool mouseMoved(const OIS::MouseEvent &arg);
    virtual bool mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
    virtual bool mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id);

	void buttonHit(OgreBites::Button *button);

	void endRound();
	void startRound();


    btDiscreteDynamicsWorld* dynamicsWorld;
    BtOgre::DebugDrawer *mDebugDrawer;

    Tower *tower;
    GameTestThing *gameTestThing;

    PlayerInput playerInput;
	irrklang::ISoundEngine *enginestart;
	int gameCount;
	bool leaderBoard;
	OgreBites::Button* b1;
	OgreBites::Button* b2;
	OgreBites::Button* b3;
	OgreBites::Button* b4;
	OgreBites::Button* butt;
	Ogre::Overlay* overlay;
	Ogre::Overlay* overlay2;
	Ogre::OverlayContainer* panel21;
	Ogre::OverlayContainer* panel31;
	
private:
    std::set<Updatable *> objects;
    Ogre::Camera* mCamera;

	
    friend class GameTestThing;
};

#endif // #ifndef __Game_h_
