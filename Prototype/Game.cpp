#include <iostream>

#include <OgreTimer.h>

#include "Game.h"
#include "Object.h"

Game::Game(void)
{
}

Game::~Game(void)
{
}

void Game::createScene(void)
{
    objects.push_back(new Object(mSceneMgr, Ogre::Vector3(50, 0, 50), "bogdan"));
    objects.push_back(new Object(mSceneMgr, Ogre::Vector3(0, 0, 0), "poo"));
    objects.push_back(new Object(mSceneMgr, Ogre::Vector3(50, 0, 0), "hello"));

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
