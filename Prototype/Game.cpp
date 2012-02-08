#include <iostream>
#include <vector>

#include <boost/bind.hpp>

#include <OGRE/OgreTimer.h>
#include <btBulletDynamicsCommon.h>
#include <BulletCollision/CollisionDispatch/btGhostObject.h>

#include "Game.h"
#include "Updatable.h"

#include "Tower.h"
#include "TowerBuilder.h"
#include "TowerPhysics.h"
#include "TowerGraphics.h"

#include "Player.h"

#include "GameTestThing.h"

Game::Game()
{
#ifdef _DEBUG
    mResourcesCfg = "resources_d.cfg";
    mPluginsCfg = "plugins_d.cfg";
#else
    mResourcesCfg = "resources.cfg";
    mPluginsCfg = "plugins.cfg";
#endif
}

Game::~Game(void)
{
}

void Game::run(void)
{    
    if (!this->setup())
    {
        return;
    }

    // Create and initialise the dynamics world
    btCollisionConfiguration *collisionConfig = new btDefaultCollisionConfiguration();

    this->dynamicsWorld = new btDiscreteDynamicsWorld(new btCollisionDispatcher(collisionConfig),
                                                      new btDbvtBroadphase(),
                                                      new btSequentialImpulseConstraintSolver(),
                                                      collisionConfig);
 
    // Required to make ghost objects work
    this->dynamicsWorld->getPairCache()->setInternalGhostPairCallback(new btGhostPairCallback());

    // Gravity is set at Earth gravity, assuming 1 unit is 1 meter
    this->dynamicsWorld->setGravity(btVector3(0, -9.8, 0));

    // Set up a debug drawer
    this->mDebugDrawer = new BtOgre::DebugDrawer(mSceneMgr->getRootSceneNode(), dynamicsWorld);
    this->dynamicsWorld->setDebugDrawer(this->mDebugDrawer);

    // camera/viewport
    mCamera = mSceneMgr->createCamera("playerCam");
    mCamera->setNearClipDistance(1);
    
    // Create one viewport, entire window
    Ogre::Viewport* vp = mWindow->addViewport(mCamera);
    vp->setBackgroundColour(Ogre::ColourValue(0,0,0));

    // Alter the camera aspect ratio to match the viewport
    mCamera->setAspectRatio(Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));

    this->gameTestThing = new GameTestThing(this);

    // Stuff
    Ogre::Timer timer;

    double oldTime = timer.getMilliseconds() / 1000.0;

    double simTime = 0;
    double simFrameLength = 0.01;

    timer.reset();
    double simTimeQueued = 0.0;

    int counted = 0;

    while (true)
    {
        Ogre::WindowEventUtilities::messagePump();

        double currentTime = timer.getMilliseconds() / 1000.0;

        double frameTime = currentTime - oldTime;

        oldTime = currentTime;

        if (frameTime > 0.25)
        {
           frameTime = 0.25;
        }

        simTimeQueued += frameTime;

        while (simTimeQueued >= simFrameLength)
        {
            //previousState = currentState;
            //integrate (physics) (currentState, simTime, dt);
            dynamicsWorld->stepSimulation(simFrameLength, 1, simFrameLength);

            for(std::set<Updatable *>::iterator i = objects.begin(); i != objects.end(); ++i)
            {
                Updatable *object = *i;

                object->update();
            }
			
            this->gameTestThing->update();

            simTime += simFrameLength;
            simTimeQueued -= simFrameLength;

            ++counted;
        }

        const double alpha = simTimeQueued / simFrameLength;

        // state = currentState*alpha + previousState * (1.0 - alpha)

        if(mWindow->isClosed())
        {
            break;
        }

        // Render a frame
        //mDebugDrawer->step();
        if (!mRoot->renderOneFrame())
        {
            break;
        }
    }

    destroyScene();

    std::cout << "You have been eaten by Bogdan!" << std::endl;
    std::cout << "*** GAME OVER ***" << std::endl;
}

//TODO: make this use changeable key bindings (not hard coded)
bool Game::keyPressed(const OIS::KeyEvent &arg)
{
    BaseApplication::keyPressed(arg);
    
    if (mTrayMgr->isDialogVisible()) return true;   // don't process any more keys if dialog is up
    
    if (arg.key == OIS::KC_UP|| arg.key == OIS::KC_W)
	{
        this->playerInput.signals.move(FORWARD, true);
	}
    else if (arg.key == OIS::KC_DOWN || arg.key == OIS::KC_S)
	{
        this->playerInput.signals.move(BACKWARD, true);
	}
    else if (arg.key == OIS::KC_LEFT || arg.key == OIS::KC_A)
	{
        this->playerInput.signals.move(LEFT, true);
	}
    else if (arg.key == OIS::KC_RIGHT || arg.key == OIS::KC_D)
	{
        this->playerInput.signals.move(RIGHT, true);
	}
	else if (arg.key == OIS::KC_SPACE)
	{
        this->playerInput.signals.jump(true);
	}
	else if (arg.key == OIS::KC_F12)
	{
		this->gameTestThing->netSendExplosion(1.0,2.0,3.0);
	}
	else if (arg.key == OIS::KC_F8)
	{
		this->gameTestThing->netStartClient();
	}
	else if (arg.key == OIS::KC_F9)
	{
		this->gameTestThing->netStartServer();
	}
	else if (arg.key == OIS::KC_F10)
	{
		this->gameTestThing->netSendChat("hello");
	}
	else if (arg.key == OIS::KC_T)
	{
		//this->objects.find(turret);
		Ogre::Vector3 min(0,0,0);
		double minDis = 1000;
		double check = 0;
		check = (Ogre::Vector3(0, 150, 400) - this->player->position).length();
		if(minDis > check) {
			min = Ogre::Vector3(0, 150, 400);
			minDis = check;
		}
	    check = (Ogre::Vector3(0, 150, -400) - this->player->position).length();
		if(minDis > check) {
			min = Ogre::Vector3(0, 150, -400);
			minDis = check;
		}
		check = (Ogre::Vector3(400, 150, 0) - this->player->position).length();
	    if(minDis > check) {
			min = Ogre::Vector3(400, 150, 0);
			minDis = check;
		}
		check = (Ogre::Vector3(-400, 150, 0) - this->player->position).length();
		if(minDis > check) {
			min = Ogre::Vector3(-400, 150, 0);
			minDis = check;
		}
		this->player->position = min;
		printf("hello\n");
	}
	
    return true;
}

bool Game::keyReleased(const OIS::KeyEvent &arg)
{
    BaseApplication::keyReleased(arg);

    switch (arg.key)
    {
    case OIS::KC_UP:
    case OIS::KC_W:
        this->playerInput.signals.move(FORWARD, false);
        break;
    case OIS::KC_DOWN:
    case OIS::KC_S:
        this->playerInput.signals.move(BACKWARD, false);
        break;
    case OIS::KC_LEFT:
    case OIS::KC_A:
        this->playerInput.signals.move(LEFT, false);
        break;
    case OIS::KC_RIGHT:
    case OIS::KC_D:
        this->playerInput.signals.move(RIGHT, false);
        break;
    case OIS::KC_SPACE:
        this->playerInput.signals.jump(false);
        break;
    default:
        break;
    }

    return true;
}

bool Game::mouseMoved(const OIS::MouseEvent &arg)
{
    BaseApplication::mouseMoved(arg);
    const OIS::MouseState &ms = mMouse->getMouseState();

    this->playerInput.signals.look(ms.X.rel, ms.Y.rel);

    return true;
}

bool Game::mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{
    BaseApplication::mousePressed(arg, id);

    if (id == OIS::MB_Right)
	{
        this->playerInput.signals.create(true);
	}
    else if (id == OIS::MB_Left)
    {
        this->playerInput.signals.fire(true);
    }

    return true;
}

bool Game::mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{
    BaseApplication::mouseReleased(arg, id);

    if (id == OIS::MB_Right)
	{
        this->playerInput.signals.create(false);
	}
    else if (id == OIS::MB_Left)
    {
        this->playerInput.signals.fire(false);
    }

    return true;
}
