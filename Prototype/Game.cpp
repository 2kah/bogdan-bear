#include <iostream>
#include <vector>

#include <boost/bind.hpp>

#include <OGRE/OgreTimer.h>
#include <btBulletDynamicsCommon.h>
#include <BulletCollision/CollisionDispatch/btGhostObject.h>

#include <OGRE/OgreOverlay.h>
#include <OGRE/OgreOverlayManager.h>
#include <OGRE/OgreTextAreaOverlayElement.h>
#include <OgreFont.h>
#include <OgreFrameListener.h>
#include <OgreStringConverter.h>

#include "Game.h"
#include "Updatable.h"

#include "Tower.h"
#include "TowerBuilder.h"
#include "TowerPhysics.h"
#include "TowerGraphics.h"

#include "Player.h"

#include "GameTestThing.h"

Game::Game()
	: game(false)
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

void Game::run(char *hostIP)
{    
    if (!this->setup())
    {
        return;
    }

    // Create and initialise the dynamics world
    /*btCollisionConfiguration *collisionConfig = new btDefaultCollisionConfiguration();

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
    this->dynamicsWorld->setDebugDrawer(this->mDebugDrawer);*/
	setUpPhysicsWorld();

    // camera/viewport
    mCamera = mSceneMgr->getCamera("playerCam");
	mCamera->setPosition(Ogre::Vector3(400,900,400));
	mCamera->setOrientation(Ogre::Quaternion(Ogre::Degree(60), Ogre::Vector3::UNIT_Y)*Ogre::Quaternion(Ogre::Degree(-45), Ogre::Vector3::UNIT_X));
    mCamera->setNearClipDistance(1);
    
    // Create one viewport, entire window
    //Ogre::Viewport* vp = mWindow->addViewport(mCamera);
    //vp->setBackgroundColour(Ogre::ColourValue(0,0,0));

    // Alter the camera aspect ratio to match the viewport
    //mCamera->setAspectRatio(Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));

    this->gameTestThing = new GameTestThing(this, hostIP);

	//---------------This builds the view at the start for the menu.
	this->gameTestThing->buildScene();

	Ogre::FontManager::getSingleton().getByName("SdkTrays/Caption")->load();
	//OgreBites::Button* b = mTrayMgr->createButton(OgreBites::TL_CENTER, "StartLocalGame", "Start Local Game");
	OgreBites::Button* b1 = mTrayMgr->createButton(OgreBites::TL_CENTER, "HostGame", "Host Game");
	OgreBites::Button* b2 = mTrayMgr->createButton(OgreBites::TL_CENTER, "JoinGame", "Join Game");
	OgreBites::Button* b3 = mTrayMgr->createButton(OgreBites::TL_CENTER, "Controls", "Controls");
    OgreBites::Button* b4 = mTrayMgr->createButton(OgreBites::TL_CENTER, "Exit", "Exit");
	mTrayMgr->showLogo(OgreBites::TL_TOPLEFT);
	//mTrayMgr->moveWidgetToTray(b, OgreBites::TL_CENTER);
	//mTrayMgr->moveWidgetToTray(b1, OgreBites::TL_CENTER);
	//mTrayMgr->moveWidgetToTray(b2, OgreBites::TL_CENTER);

    // Stuff
    Ogre::Timer timer;

    double oldTime = timer.getMilliseconds() / 1000.0;

    double simTime = 0;
    double simFrameLength = 0.01;

    timer.reset();
    double simTimeQueued = 0.0;

    int counted = 0;
	this->gameCount = 0;
	this->leaderBoard = false;
	//bool game = true;
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
			this->gameTestThing->network->update();
            //previousState = currentState;
            //integrate (physics) (currentState, simTime, dt);
			//---------------This stops everything getting updated once the game has finished.
			if(game == true && leaderBoard == false) {

                dynamicsWorld->stepSimulation(simFrameLength, 1, simFrameLength);

                for(std::set<Updatable *>::iterator i = objects.begin(); i != objects.end(); ++i)
                {
                    Updatable *object = *i;

                    object->update();
                }
			
                this->gameTestThing->update();
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
        //mDebugDrawer->step();
        if (!mRoot->renderOneFrame())
        {
            break;
        }

		if(this->gameTestThing->goal != NULL) 
		{
			if(this->gameTestThing->goal->isGameOver()) {
				//This prints the stuff once if the game has ended and then you can press enter to restart a new game
				if(game == true) 
				{
					endRound();
					std::cout << "You have been eaten by Bogdan!" << std::endl;
					std::cout << "*** GAME OVER ***" << std::endl;
					this->gameCount++;
				    game = false;
					this->leaderBoard = true;
				}
			}
		}
    }

}

//TODO: make this use changeable key bindings (not hard coded)
bool Game::keyPressed(const OIS::KeyEvent &arg)
{
    BaseApplication::keyPressed(arg);
    
    if (mTrayMgr->isDialogVisible()) return true;   // don't process any more keys if dialog is up
    
	if (arg.key == OIS::KC_RETURN && this->gameTestThing->network->hosting && game == false) {
		if(!this->leaderBoard) {
		    this->gameTestThing->network->sendStartGame();
		    game = true;
		    this->gameTestThing->network->sendExplosion((Ogre::Vector3(0, 240, 0)),true);
		}
		else {
			startRound();
	        this->mRoot->clearEventTimes();
	        this->gameTestThing->destroyScene();
	        this->gameTestThing->resetScores();
			this->gameTestThing->network->sendStartGame();
		    this->gameTestThing->startNewRoundServer();
			for(int i = 0; i < 10000; i++) {
			    //printf("%i\n", i);
		    }
			leaderBoard = false;
		}
	}

	if (!game) return true;

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
		//this->gameTestThing->netSendExplosion(1.0,2.0,3.0);
	}
	else if (arg.key == OIS::KC_F8)
	{
		this->gameTestThing->startClient();
		mTrayMgr->hideCursor();
	}
	else if (arg.key == OIS::KC_F9)
	{
		this->gameTestThing->startServer();
		mTrayMgr->hideCursor();
	}
	else if (arg.key == OIS::KC_F10)
	{
		printf("%d - %d = %d\n",sizeof(NetTower),420000,sizeof(NetTower)-420000);
		//this->gameTestThing->resetTower();
    }
    else if (arg.key == OIS::KC_F11)
	{
		this->gameTestThing->startLocal();
		mTrayMgr->hideCursor();
    }
    //else if (arg.key == OIS::KC_T)
    //{
    //    this->playerInput.signals.use(true);
	//}
	//TODO: remove these 2 (testing purposes only)
	else if (arg.key == OIS::KC_1)
	{
		this->playerInput.signals.deactivate(true);
	}
	else if (arg.key == OIS::KC_2)
	{
		this->playerInput.signals.reactivate(true);
	}
	else if (arg.key == OIS::KC_E)
    {
        this->playerInput.signals.use(true);
	}

    return true;
}

bool Game::keyReleased(const OIS::KeyEvent &arg)
{
    BaseApplication::keyReleased(arg);
	
	if (!game) return true;
	
	//Ogre::Overlay *ol;
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
	if (mTrayMgr->injectMouseDown(arg, id)) return true;
    
	if (!game) return true;
	
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

	if (!game) return true;

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

//This is for the menu buttons
void Game::buttonHit(OgreBites::Button *button)
{
    if(button->getName() == "StartLocalGame")
	{
		//startRound();
	}
    else if(button->getName() == "JoinGame")
	{
		//this->gameTestThing->destroyScene();
		startRound();
	    this->mRoot->clearEventTimes();
	    this->gameTestThing->destroyScene();
		this->gameTestThing->startClient();
		mTrayMgr->hideCursor();
		mTrayMgr->destroyAllWidgets();
		mTrayMgr->clearAllTrays();
	}
	else if(button->getName() == "HostGame")
	{
		//this->gameTestThing->destroyScene();
		startRound();
	    this->mRoot->clearEventTimes();
	    this->gameTestThing->destroyScene();
		this->gameTestThing->startServer();
		mTrayMgr->hideCursor();
		mTrayMgr->destroyAllWidgets();
		mTrayMgr->clearAllTrays();
	}
}

//This would show an overlay with scores on and who has won the game and set the camera up in the sky for a nice frozen view of the end of the game
void Game::endRound()
{
	//this->gameTestThing->destroyScene();

	Ogre::OverlayManager& overlayManager = Ogre::OverlayManager::getSingleton();

	overlayManager.destroyAllOverlayElements();
	overlayManager.getByName("OverlayName")->clear();
    
    // Create a panel
    Ogre::OverlayContainer* panel2 = static_cast<Ogre::OverlayContainer*>(
    overlayManager.createOverlayElement("Panel", "PanelName2"));
    panel2->setMetricsMode(Ogre::GMM_PIXELS);
    panel2->setPosition(10, 10);
	//panel2->_setDimensions(200, 200);
	panel2->setDimensions(1000, 1000);
    
	/*Ogre::TextAreaOverlayElement* textArea = static_cast<Ogre::TextAreaOverlayElement*>(
    overlayManager.createOverlayElement("TextArea", "win/lose"));
    //textArea->setFontName("BlueHighway");
    textArea->setCharHeight(16);
    textArea->setColour(Ogre::ColourValue(0.3, 0.5, 0.3));
    textArea->setCaption("Hello OGRE!");
	printf("here\n");
    panel2->addChild(textArea);*/
	//Ogre::TextAreaOverlayElement* textArea2 = static_cast<Ogre::TextAreaOverlayElement*>(overlayManager.createOverlayElement("TextArea2", "TextAreaName2"));
	Ogre::FontPtr mFont = Ogre::FontManager::getSingleton().create("MyFont", "General");
    mFont->setType(Ogre::FT_TRUETYPE);
    mFont->setSource("solo5.ttf");
    mFont->setTrueTypeSize(26);
    mFont->setTrueTypeResolution(96);
	mFont->load();

	Ogre::OverlayElement* teamWins = overlayManager.createOverlayElement("TextArea", "teamWins");
    teamWins->setMetricsMode(Ogre::GMM_PIXELS);
    teamWins->setPosition(0, 0);
    teamWins->setDimensions(mWindow->getWidth(), (mWindow->getHeight()/4));
	teamWins->setParameter("char_height", "55");
	teamWins->setParameter("font_name", "MyFont");
    teamWins->setCaption(this->gameTestThing->winningTeam + " TEAM WINS");
    teamWins->setColour(Ogre::ColourValue(0, 0.984, 0.925));

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	float time;
	Ogre::OverlayElement* redTeamPoints = overlayManager.createOverlayElement("TextArea", "redTeamPoints");
    redTeamPoints->setMetricsMode(Ogre::GMM_PIXELS);
    redTeamPoints->setPosition(0, (mWindow->getHeight()/4)*2);
    redTeamPoints->setDimensions((mWindow->getWidth()/4), (mWindow->getHeight()/4));
	redTeamPoints->setParameter("char_height", "26");
	redTeamPoints->setParameter("font_name", "MyFont");
	time = this->gameTestThing->network->teamScores[0];
    redTeamPoints->setCaption(Ogre::StringConverter::toString(this->gameTestThing->network->teamScores[0]));
    redTeamPoints->setColour(Ogre::ColourValue(0, 0.984, 0.925));

	Ogre::OverlayElement* orangeTeamPoints = overlayManager.createOverlayElement("TextArea", "orangeTeamPoints");
    orangeTeamPoints->setMetricsMode(Ogre::GMM_PIXELS);
    orangeTeamPoints->setPosition((mWindow->getWidth()/4), (mWindow->getHeight()/4)*2);
    orangeTeamPoints->setDimensions((mWindow->getWidth()/4), (mWindow->getHeight()/4));
	orangeTeamPoints->setParameter("char_height", "26");
	orangeTeamPoints->setParameter("font_name", "MyFont");
	time = this->gameTestThing->network->teamScores[1];
    orangeTeamPoints->setCaption(Ogre::StringConverter::toString(this->gameTestThing->network->teamScores[1]));
    orangeTeamPoints->setColour(Ogre::ColourValue(0, 0.984, 0.925));

	Ogre::OverlayElement* purpleTeamPoints = overlayManager.createOverlayElement("TextArea", "purpleTeamPoints");
    purpleTeamPoints->setMetricsMode(Ogre::GMM_PIXELS);
    purpleTeamPoints->setPosition((mWindow->getWidth()/4)*2, (mWindow->getHeight()/4)*2);
    purpleTeamPoints->setDimensions((mWindow->getWidth()/4), (mWindow->getHeight()/4));
	purpleTeamPoints->setParameter("char_height", "26");
	purpleTeamPoints->setParameter("font_name", "MyFont");
	time = this->gameTestThing->network->teamScores[2];
    orangeTeamPoints->setCaption(Ogre::StringConverter::toString(this->gameTestThing->network->teamScores[2]));
    purpleTeamPoints->setColour(Ogre::ColourValue(0, 0.984, 0.925));

	Ogre::OverlayElement* greenTeamPoints = overlayManager.createOverlayElement("TextArea", "greenTeamPoints");
    greenTeamPoints->setMetricsMode(Ogre::GMM_PIXELS);
    greenTeamPoints->setPosition((mWindow->getWidth()/4)*3, (mWindow->getHeight()/4)*2);
    greenTeamPoints->setDimensions((mWindow->getWidth()/4), (mWindow->getHeight()/4));
	greenTeamPoints->setParameter("char_height", "26");
	greenTeamPoints->setParameter("font_name", "MyFont");
	time = this->gameTestThing->network->teamScores[3];
    orangeTeamPoints->setCaption(Ogre::StringConverter::toString(this->gameTestThing->network->teamScores[3]));
    greenTeamPoints->setColour(Ogre::ColourValue(0, 0.984, 0.925));

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	Ogre::OverlayElement* redTeamWins = overlayManager.createOverlayElement("TextArea", "redTeamWins");
    redTeamWins->setMetricsMode(Ogre::GMM_PIXELS);
    redTeamWins->setPosition(0, (mWindow->getHeight()/4)*3);
    redTeamWins->setDimensions((mWindow->getWidth()/4), (mWindow->getHeight()/4));
	redTeamWins->setParameter("char_height", "50");
	redTeamWins->setParameter("font_name", "MyFont");
    redTeamWins->setCaption(Ogre::StringConverter::toString(this->gameTestThing->wins[0]));
    redTeamWins->setColour(Ogre::ColourValue(0, 0.984, 0.925));

	Ogre::OverlayElement* orangeTeamWins = overlayManager.createOverlayElement("TextArea", "orangeTeamWins");
    orangeTeamWins->setMetricsMode(Ogre::GMM_PIXELS);
    orangeTeamWins->setPosition((mWindow->getWidth()/4), (mWindow->getHeight()/4)*3);
    orangeTeamWins->setDimensions((mWindow->getWidth()/4), (mWindow->getHeight()/4));
	orangeTeamWins->setParameter("char_height", "50");
	orangeTeamWins->setParameter("font_name", "MyFont");
    orangeTeamWins->setCaption(Ogre::StringConverter::toString(this->gameTestThing->wins[1]));
    orangeTeamWins->setColour(Ogre::ColourValue(0, 0.984, 0.925));

	Ogre::OverlayElement* purpleTeamWins = overlayManager.createOverlayElement("TextArea", "purpleTeamWins");
    purpleTeamWins->setMetricsMode(Ogre::GMM_PIXELS);
    purpleTeamWins->setPosition((mWindow->getWidth()/4)*2, (mWindow->getHeight()/4)*3);
    purpleTeamWins->setDimensions((mWindow->getWidth()/4), (mWindow->getHeight()/4));
	purpleTeamWins->setParameter("char_height", "50");
	purpleTeamWins->setParameter("font_name", "MyFont");
    purpleTeamWins->setCaption(Ogre::StringConverter::toString(this->gameTestThing->wins[2]));
    purpleTeamWins->setColour(Ogre::ColourValue(0, 0.984, 0.925));

	Ogre::OverlayElement* greenTeamWins = overlayManager.createOverlayElement("TextArea", "greenTeamWins");
    greenTeamWins->setMetricsMode(Ogre::GMM_PIXELS);
    greenTeamWins->setPosition((mWindow->getWidth()/4)*3, (mWindow->getHeight()/4)*3);
    greenTeamWins->setDimensions((mWindow->getWidth()/4), (mWindow->getHeight()/4));
	greenTeamWins->setParameter("char_height", "50");
	greenTeamWins->setParameter("font_name", "MyFont");
    greenTeamWins->setCaption(Ogre::StringConverter::toString(this->gameTestThing->wins[3]));
    greenTeamWins->setColour(Ogre::ColourValue(0, 0.984, 0.925));

	Ogre::MaterialPtr crosshair2 = Ogre::MaterialManager::getSingleton().create("crosshair2", "General");
    crosshair2->getTechnique(0)->getPass(0)->createTextureUnitState("grass_1024.jpg");
    //crosshair2->getTechnique(0)->getPass(0)->setDepthCheckEnabled(true);
    crosshair2->getTechnique(0)->getPass(0)->setSceneBlending(Ogre::SBT_TRANSPARENT_ALPHA);
    //panel2->setMaterialName("crosshair2");
    
	// Create an overlay, and add the panel
    Ogre::Overlay* overlay = overlayManager.getByName("OverlayName");
    overlay->add2D(panel2);

	panel2->addChild(teamWins);
	panel2->addChild(redTeamPoints);
	panel2->addChild(orangeTeamPoints);
	panel2->addChild(purpleTeamPoints);
	panel2->addChild(greenTeamPoints);
	panel2->addChild(redTeamWins);
	panel2->addChild(orangeTeamWins);
	panel2->addChild(purpleTeamWins);
	panel2->addChild(greenTeamWins);

    // Show the overlay
    overlay->show();

	// camera/viewport
	//this->gameTestThing->player->enteredTurret();
    mCamera = mSceneMgr->getCamera("playerCam");
	mCamera->setPosition(Ogre::Vector3(400,900,400));
	mCamera->setOrientation(Ogre::Quaternion(Ogre::Degree(60), Ogre::Vector3::UNIT_Y)*Ogre::Quaternion(Ogre::Degree(-45), Ogre::Vector3::UNIT_X));
    mCamera->setNearClipDistance(1);
}

//This destroys what is there from the previous round and displays a "crosshair" (single pixel) in the middle.
void Game::startRound()
{
	this->gameTestThing->destroyLocalPlayer();
	this->gameTestThing->resetScores();
	//setUpPhysicsWorld();
	//this->gameTestThing->startLocal();
	mTrayMgr->hideCursor();
    mTrayMgr->destroyAllWidgets();
	mTrayMgr->clearAllTrays();

	Ogre::OverlayManager& overlayManager = Ogre::OverlayManager::getSingleton();

	overlayManager.destroyAllOverlayElements();

	if(overlayManager.getByName("OverlayName") != NULL)
	{
		overlayManager.getByName("OverlayName")->clear();
	}
    // Create a panel
    Ogre::OverlayContainer* panel = static_cast<Ogre::OverlayContainer*>(
    overlayManager.createOverlayElement("Panel", "PanelName"));
    panel->setMetricsMode(Ogre::GMM_PIXELS);
    panel->setPosition((mWindow->getWidth()/2), (mWindow->getHeight()/2));
    //panel->_setDimensions((mWindow->getWidth()/64), (mWindow->getHeight()/64));
    
    Ogre::MaterialPtr crosshair = Ogre::MaterialManager::getSingleton().create("crosshair", "General");
    crosshair->getTechnique(0)->getPass(0)->createTextureUnitState("crosshair.png");
    crosshair->getTechnique(0)->getPass(0)->setDepthCheckEnabled(true);
    crosshair->getTechnique(0)->getPass(0)->setSceneBlending(Ogre::SBT_TRANSPARENT_ALPHA);
    panel->setMaterialName("crosshair");
    
    // Create an overlay, and add the panel
	Ogre::Overlay* overlay = NULL;
	if(overlayManager.getByName("OverlayName") == NULL)
	{
        overlay = overlayManager.create("OverlayName");
	}
	else
	{
		overlay = overlayManager.getByName("OverlayName");
	}
    overlay->add2D(panel);
    
    // Show the overlay
	if(!overlay->isVisible()) 
	{
        overlay->show();
	}
}

//This hse just been moved from the beginning of the game loop to make stuff clearer.
void Game::setUpPhysicsWorld()
{

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
}

void Game::restartClient()
{
	if(this->leaderBoard) {
		startRound();
	    this->mRoot->clearEventTimes();
	    this->gameTestThing->destroyScene();
		//for(int i = 0; i < 10000; i++) {
			//printf("%i\n", i);
		//}
		this->gameTestThing->startNewRoundClient();
		mTrayMgr->hideCursor();
		mTrayMgr->destroyAllWidgets();
		mTrayMgr->clearAllTrays();
		this->leaderBoard = false;
		//printf("WAIT AT TOP...\n");
	}
	else {
		//printf("START GAME FOR LOCAL\n");
	    this->game = true;
	}
}