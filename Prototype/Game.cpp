#include <iostream>
#include <vector>

#include <OgreTimer.h>
#include <btBulletDynamicsCommon.h>

#include "Game.h"
#include "Object.h"
#include "SceneObject.h"
#include "Player.h"
#include "Explosion.h"

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

    things.push_back(new SceneObject(Ogre::Vector3(50, 0, 50)));
    things.push_back(new SceneObject(Ogre::Vector3(0, 0, 0)));
    things.push_back(new SceneObject(Ogre::Vector3(50, 0, 0)));
    things.push_back(player);

    // Add all the scene objects to the scene and list of objects
    for (std::vector<SceneObject *>::size_type i = 0; i != things.size(); ++i)
    {
        SceneObject *thing = things[i];

        std::stringstream name("");
        name << (int) i;

        thing->addToScene(mSceneMgr, name.str());
        objects.push_back(thing);
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

    if (!setup())
    {
        return;
    }

    btBroadphaseInterface* broadphase = new btDbvtBroadphase();
 
    btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();
    btCollisionDispatcher* dispatcher = new btCollisionDispatcher(collisionConfiguration);
 
    btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver;
 
    btDiscreteDynamicsWorld* dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher,broadphase,solver,collisionConfiguration);
 
    dynamicsWorld->setGravity(btVector3(0,-10,0));

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

            // update all the known objects
            for (std::vector<Object *>::size_type i = 0; i != objects.size(); ++i)
            {
                Object *object = objects[i];

                object->update();
            }

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

bool Game::keyPressed(const OIS::KeyEvent &arg)
{
    BaseApplication::keyPressed(arg);
    
    if (mTrayMgr->isDialogVisible()) return true;   // don't process any more keys if dialog is up
    
    if (arg.key == OIS::KC_UP)
	{
        player->forward();
	}
    else if (arg.key == OIS::KC_DOWN)
	{
        player->back();
	}
    else if (arg.key == OIS::KC_LEFT)
	{
        player->left();
	}
    else if (arg.key == OIS::KC_RIGHT)
	{
        player->right();
	}
	else if (arg.key == OIS::KC_SPACE)
	{
        player->jump();
	}
    else if (arg.key == OIS::KC_W)
	{
        player->shoot();

        Explosion *explosion = new Explosion(player->position);

        explosion->addToScene(mSceneMgr, "explosion");

        objects.push_back(explosion);
	}
    else if (arg.key == OIS::KC_LSHIFT)
	{
        player->platform();
	}
    
    return true;
}

bool Game::keyReleased(const OIS::KeyEvent &arg)
{
    BaseApplication::keyReleased(arg);

    return true;
}
