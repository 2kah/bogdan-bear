//#define __USE_OLD_TOWER__

#include <iostream>
#include <vector>

#include <boost/bind.hpp>

#include <OGRE/OgreTimer.h>
#include <btBulletDynamicsCommon.h>

#include "Game.h"
#include "Updatable.h"

#include "Tower.h"
#include "TowerBuilder.h"
#include "TowerPhysics.h"
#include "TowerGraphics.h"

#include "SceneObject.h"
#include "Player.h"
#include "FallingObject.h"

#include "GameTestThing.h"

Game::Game(void)
{
}

Game::~Game(void)
{
}

void Game::createScene(void)
{    
    //this code is in wrong place
    btBroadphaseInterface* broadphase = new btDbvtBroadphase();
 
    btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();
    btCollisionDispatcher* dispatcher = new btCollisionDispatcher(collisionConfiguration);
 
    btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver;
 
    dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher,broadphase,solver,collisionConfiguration);
 
    dynamicsWorld->setGravity(btVector3(0, -9.8, 0));

#ifdef __USE_OLD_TOWER__
    TowerOld *tower = new TowerOld(mSceneMgr, dynamicsWorld);
#else
    tower = new Tower(2.0, 128, 16, 32);

    TowerBuilder *builder = new TowerBuilder(tower);
    builder->regenerate();

    this->objects.insert(builder);

    TowerGraphics *towerGraphics = new TowerGraphics(tower, mSceneMgr);
    TowerPhysics *towerPhysics = new TowerPhysics(tower, dynamicsWorld);
#endif

    this->mDebugDrawer = new BtOgre::DebugDrawer(mSceneMgr->getRootSceneNode(), dynamicsWorld);
    this->dynamicsWorld->setDebugDrawer(this->mDebugDrawer);

    // Create a list of SceneObjects (one of which is a player) at various positions
    std::vector<SceneObject *> things;

    fallingObject = new FallingObject(Ogre::Vector3(0, 200 / 16.0, 0));

    this->gameTestThing = new GameTestThing(this);
    
    //things.push_back(player);
    things.push_back(fallingObject);

    // Add all the scene objects to the scene and list of objects
    for (std::vector<SceneObject *>::size_type i = 0; i != things.size(); ++i)
    {
        SceneObject *thing = things[i];

        thing->addToScene(mSceneMgr);
        thing->addToPhysics(dynamicsWorld);
        objects.insert(thing);
    }

    // Set ambient light
    mSceneMgr->setAmbientLight(Ogre::ColourValue(0.9, 0.9, 0.9));
	    //TODO: replace with actual lighting
    Ogre::Light* pointlight1 = mSceneMgr->createLight("pointlight1");
    pointlight1->setType(Ogre::Light::LT_POINT);
    pointlight1->setPosition(Ogre::Vector3(500, 150, 0));
    Ogre::Light* pointlight2 = mSceneMgr->createLight("pointlight2");
    pointlight2->setType(Ogre::Light::LT_POINT);
    pointlight2->setPosition(Ogre::Vector3(-500, 150, 0));
 
 
    // Create a light
    //Ogre::Light* l = mSceneMgr->createLight("MainLight");
    //l->setPosition(20,80,50);

}

void Game::run(void)
{
#ifdef _DEBUG
    mResourcesCfg = "resources_d.cfg";
    mPluginsCfg = "plugins_d.cfg";
#else
    mResourcesCfg = "resources.cfg";
    mPluginsCfg = "plugins.cfg";
#endif
    if (!setup())
    {
        return;
    }

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
        mDebugDrawer->step();
        if (!mRoot->renderOneFrame())
        {
            break;
        }
    }

    destroyScene();

    std::cout << "You have been eaten by Bogdan!" << std::endl;
    std::cout << "*** GAME OVER ***" << std::endl;
}
//TODO: put this code where it should be
void Game::createCamera(void)
{
    mCamera = mSceneMgr->createCamera("playerCam");
    mCamera->setNearClipDistance(1);
}

void Game::createViewports(void)
{
    // Create one viewport, entire window
    Ogre::Viewport* vp = mWindow->addViewport(mCamera);
    vp->setBackgroundColour(Ogre::ColourValue(0,0,0));

    // Alter the camera aspect ratio to match the viewport
    mCamera->setAspectRatio(Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));
}

void Game::carveSphere(Ogre::Vector3 position, double radius)
{
    std::cout << "(Game) Carving sphere at " << position << " of radius " << radius << std::endl;

    this->tower->carveSphere(position, radius);
}

bool Game::keyPressed(const OIS::KeyEvent &arg)
{
    BaseApplication::keyPressed(arg);
    
    if (mTrayMgr->isDialogVisible()) return true;   // don't process any more keys if dialog is up
    
    if (arg.key == OIS::KC_UP|| arg.key == OIS::KC_W)
	{
        this->playerInput.signals.move(DIRECTION::FORWARD, true);
	}
    else if (arg.key == OIS::KC_DOWN || arg.key == OIS::KC_S)
	{
        this->playerInput.signals.move(DIRECTION::BACKWARD, true);
	}
    else if (arg.key == OIS::KC_LEFT || arg.key == OIS::KC_A)
	{
        this->playerInput.signals.move(DIRECTION::LEFT, true);
	}
    else if (arg.key == OIS::KC_RIGHT || arg.key == OIS::KC_D)
	{
        this->playerInput.signals.move(DIRECTION::RIGHT, true);
	}
	else if (arg.key == OIS::KC_SPACE)
	{
        this->playerInput.signals.jump(true);
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
        this->playerInput.signals.move(DIRECTION::FORWARD, false);
        break;
    case OIS::KC_DOWN:
    case OIS::KC_S:
        this->playerInput.signals.move(DIRECTION::BACKWARD, false);
        break;
    case OIS::KC_LEFT:
    case OIS::KC_A:
        this->playerInput.signals.move(DIRECTION::LEFT, false);
        break;
    case OIS::KC_RIGHT:
    case OIS::KC_D:
        this->playerInput.signals.move(DIRECTION::RIGHT, false);
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
#ifdef __USE_OLD_TOWER__
        player->shoot(mSceneMgr, dynamicsWorld, this->tower);
#endif
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
