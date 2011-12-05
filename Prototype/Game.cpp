#include <iostream>
#include <vector>

#include <OgreTimer.h>
#include <btBulletDynamicsCommon.h>

#include "Game.h"
#include "Object.h"
#include "Tower.h"
#include "SceneObject.h"
#include "Player.h"
#include "Explosion.h"
#include "FallingObject.h"

Game::Game(void)
{
}

Game::~Game(void)
{
}

void Game::createScene(void)
{
    // Create a list of SceneObjects (one of which is a player) at various positions
    std::vector<SceneObject *> things;

    player = new Player(Ogre::Vector3(100, 0, 100));
    fallingObject = new FallingObject(Ogre::Vector3(0,100,0));

    /*things.push_back(new SceneObject(Ogre::Vector3(50, 0, 50)));
    things.push_back(new SceneObject(Ogre::Vector3(0, 0, 0)));
    things.push_back(new SceneObject(Ogre::Vector3(50, 0, 0)));*/
    things.push_back(player);
    //things.push_back(fallingObject);

    // Add all the scene objects to the scene and list of objects
    for (std::vector<SceneObject *>::size_type i = 0; i != things.size(); ++i)
    {
        SceneObject *thing = things[i];

        std::stringstream name("");
        name << (int) i;

        thing->addToScene(mSceneMgr, name.str());
        thing->addToPhysics(dynamicsWorld);
        objects.insert(thing);
    }

    // Set ambient light
    mSceneMgr->setAmbientLight(Ogre::ColourValue(0.5, 0.5, 0.5));
 
    // Create a light
    Ogre::Light* l = mSceneMgr->createLight("MainLight");
    l->setPosition(20,80,50);
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

    //this code is in wrong place
    btBroadphaseInterface* broadphase = new btDbvtBroadphase();
 
    btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();
    btCollisionDispatcher* dispatcher = new btCollisionDispatcher(collisionConfiguration);
 
    btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver;
 
    dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher,broadphase,solver,collisionConfiguration);
 
    dynamicsWorld->setGravity(btVector3(0,-1,0));
    
    if (!setup())
    {
        return;
    }
    
    tower = new Tower(mSceneMgr);// To build a tower

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

            for(std::set<Object *>::iterator i = objects.begin(); i != objects.end(); ++i)
            {
                Object *object = *i;

                object->update();
            }

            for(std::set<SceneObject *>::iterator i = sceneRemoveQueue.begin(); i != sceneRemoveQueue.end(); ++i)
            {
                SceneObject *object = *i;

                objects.erase(object);
                object->removeFromScene(mSceneMgr);
            }

            sceneRemoveQueue.clear();

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
        if (!mRoot->renderOneFrame())
        {
            break;
        }
    }

    destroyScene();

    std::cout << "You have been eaten by Bogdan!" << std::endl;
    std::cout << "*** GAME OVER ***" << std::endl;
}

void Game::removeSceneObject(SceneObject *object)
{
    sceneRemoveQueue.insert(object);
}

void Game::carveSphere(Ogre::Vector3 position, double radius)
{
    std::cout << "Carving sphere at " << position << " of radius " << radius << std::endl;
}

bool Game::keyPressed(const OIS::KeyEvent &arg)
{
    BaseApplication::keyPressed(arg);
    
    if (mTrayMgr->isDialogVisible()) return true;   // don't process any more keys if dialog is up
    
    //may be nicer as a switch case
    if (arg.key == OIS::KC_UP || arg.key == OIS::KC_W)
	{
        player->forward();
	}
    else if (arg.key == OIS::KC_DOWN || arg.key == OIS::KC_S)
	{
        player->back();
	}
    else if (arg.key == OIS::KC_LEFT || arg.key == OIS::KC_A)
	{
        player->left();
	}
    else if (arg.key == OIS::KC_RIGHT || arg.key == OIS::KC_D)
	{
        player->right();
	}
	else if (arg.key == OIS::KC_SPACE)
	{
        player->jump();
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
        player->stopMovingForward();
        break;
    case OIS::KC_DOWN:
    case OIS::KC_S:
        player->stopMovingBack();
        break;
    case OIS::KC_LEFT:
    case OIS::KC_A:
        player->stopMovingLeft();
        break;
    case OIS::KC_RIGHT:
    case OIS::KC_D:
        player->stopMovingRight();
        break;
    default:
        break;
    }

    return true;
}

bool Game::mouseMoved(const OIS::MouseEvent &arg)
{
    BaseApplication::mouseMoved(arg);
    return true;
}

bool Game::mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{
    BaseApplication::mousePressed(arg, id);

    if (id == OIS::MB_Right)
	{
        player->platform();
	}
    else if (id == OIS::MB_Left)
    {
        static int i = 0;
        
        std::stringstream name("explosion");
        name << (int) i;

        ++i;

        player->shoot();

        Explosion *explosion = new Explosion(this, player->position);

        explosion->addToScene(mSceneMgr, name.str());

        objects.insert(explosion);
    }

    return true;
}

bool Game::mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{
    BaseApplication::mouseReleased(arg, id);
    return true;
}
