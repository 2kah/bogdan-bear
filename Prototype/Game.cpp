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
	: game(false), serverGameStarted(false)
{
#ifdef _DEBUG
    mResourcesCfg = "resources_d.cfg";
    mPluginsCfg = "plugins_d.cfg";
#else
    mResourcesCfg = "resources.cfg";
    mPluginsCfg = "plugins.cfg";
#endif
}

bool optionsmenu = FALSE;
bool alreadyshown = FALSE;
bool endofRound = FALSE;

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
	 b1 = mTrayMgr->createButton(OgreBites::TL_CENTER, "HostGame", "Host Game");
	 b2 = mTrayMgr->createButton(OgreBites::TL_CENTER, "JoinGame", "Join Game");
	 b3 = mTrayMgr->createButton(OgreBites::TL_CENTER, "Controls", "Controls");
     b4 = mTrayMgr->createButton(OgreBites::TL_CENTER, "Exit", "Exit");
	mTrayMgr->showLogo(OgreBites::TL_TOPLEFT);
	//mTrayMgr->moveWidgetToTray(b, OgreBites::TL_CENTER);
	//mTrayMgr->moveWidgetToTray(b1, OgreBites::TL_CENTER);
	//mTrayMgr->moveWidgetToTray(b2, OgreBites::TL_CENTER);

	Ogre::OverlayManager& overlayManager = Ogre::OverlayManager::getSingleton();
	panel21 = static_cast<Ogre::OverlayContainer*>(
	overlayManager.createOverlayElement("Panel", "PanelName21"));
	panel21->setMetricsMode(Ogre::GMM_PIXELS);
	panel21->setPosition(0, 0);
	panel21->_setDimensions(1, 1);

	//Ogre::OverlayManager& overlayManager2 = Ogre::OverlayManager::getSingleton();
	panel31 = static_cast<Ogre::OverlayContainer*>(
			overlayManager.createOverlayElement("Panel", "PanelName22"));

	Ogre::MaterialPtr crosshair = Ogre::MaterialManager::getSingleton().create("crosshair", "General");
	crosshair->getTechnique(0)->getPass(0)->createTextureUnitState("controls.png");
	panel21->setMaterialName("crosshair");

	butt = mTrayMgr->createButton(OgreBites::TL_BOTTOMRIGHT, "Back", "Back to Main Menu");
	butt->hide();	

	this->enginestart = irrklang::createIrrKlangDevice();
	this->enginestart->setSoundVolume(0.5f);
	this->enginestart->play2D("sounds/menu.mp3", true);

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
	this->interRoundTimer = 0;
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
			//start new round x seconds after old one ended
			else if(this->serverGameStarted && this->gameTestThing->network->hosting)
			{
				if(this->interRoundTimer < 1500)
					this->interRoundTimer++;
				//remove the leader board and put players in spawn locations after 10 secs
				if(this->interRoundTimer == 1000)
				{
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
				//then start the game after waiting another 5 secs
				else if(this->interRoundTimer == 1500)
				{
					this->gameTestThing->network->sendStartGame();
					game = true;
					this->gameTestThing->network->sendExplosion((Ogre::Vector3(0, 240, 0)),true);

					this->interRoundTimer = 0;
				}
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
					//std::cout << "You have been eaten by Bogdan!" << std::endl;
					//std::cout << "*** GAME OVER ***" << std::endl;
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
  	Ogre::OverlayManager& overlayManager = Ogre::OverlayManager::getSingleton();
    if (mTrayMgr->isDialogVisible()) return true;   // don't process any more keys if dialog is up
    
	if (arg.key == OIS::KC_RETURN && this->gameTestThing->network->hosting && game == false) {
		if(!this->leaderBoard) {
			//then this
		    this->gameTestThing->network->sendStartGame();
		    game = true;
		    this->gameTestThing->network->sendExplosion((Ogre::Vector3(0, 240, 0)),true);
			this->serverGameStarted = true;
		}
		else {
			//server should do this after x time
			startRound();
			mTrayMgr->hideCursor();
			mTrayMgr->destroyAllWidgets();
			mTrayMgr->clearAllTrays();
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
	if (arg.key == OIS::KC_ESCAPE)
    {	
		if(!this->leaderBoard) {
		    if(!optionsmenu && !endofRound){
		    	optionsmenu = true;
		    	//overlayManager.getByName("OverlayName")->clear();
		    	mTrayMgr->clearAllTrays();
		    	mTrayMgr->destroyAllWidgets();
		    	//mTrayMgr->hideAll();
	            //if(mTrayMgr->getListener() == NULL) printf("mTrayMgr has lost its listener\n");
		    	//printf("hereeeeee in if \n");
		    	//mTrayMgr->createButton(OgreBites::TL_CENTER, "Exit2", "Exit Game");
		    	//printf("kjgkgj\n");
		    	//mTrayMgr->createButton(OgreBites::TL_CENTER, "Controls2", "Show Controls");
		    	//mTrayMgr->createLabel(OgreBites::TL_CENTER,"info","Press Escape to return to game");
		    	//printf("gets here but \n");
		    	//In game pause menu
		    	
		    	panel31->setMetricsMode(Ogre::GMM_PIXELS);
		    	panel31->setPosition(10, 10);
		    	panel31->_setDimensions(0.95, 0.95);
		    	
		    	Ogre::MaterialPtr options = Ogre::MaterialManager::getSingleton().create("options", "General");
		    	options->getTechnique(0)->getPass(0)->createTextureUnitState("controls.png");
		    	panel31->setMaterialName("options");
		    	
		    	//mTrayMgr->showAll();
		    
		    	//alreadyshown = TRUE;
		        mTrayMgr->clearAllTrays();
		        mTrayMgr->destroyAllWidgets();
		        //butt->show();
		        // Create an overlay, and add the panel
		        if(overlayManager.getByName("OverlayName") == NULL)
		        {
		        	overlay2 = overlayManager.create("OverlayName");
		        }
		        else
		        {
		        	overlay2 = overlayManager.getByName("OverlayName");
		        }
		        overlay2->setZOrder(200);
		        overlay2->add2D(panel31);
		        // Show the overlay
		        if(!overlay2->isVisible()) 
		        {
		        	overlay2->show();	
		        }
		    }
		    else{
		    	//if(!alreadyshown){
		            overlayManager.getByName("OverlayName")->clear();
		    		mTrayMgr->destroyAllWidgets();
		    		mTrayMgr->clearAllTrays();
		    		//mTrayMgr->hideCursor();
		    		optionsmenu = false;
		    		Ogre::Overlay* overlay2 = NULL;
		    		if(overlayManager.getByName("OverlayName") == NULL)
		    		{
		    		    overlay2 = overlayManager.create("OverlayName");
		    		}
		    		else
		    		{
		    			overlay2 = overlayManager.getByName("OverlayName");
		    		}
		    		overlay2->add2D(panel);
		    		
		    		// Show the overlay
		    		if(!overlay2->isVisible()) 
		    		{
		    		    overlay2->show();
		    		}
		    	//}
		    	
		    }
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
	//else if (arg.key == OIS::KC_F12)
	//{
	//	//this->gameTestThing->netSendExplosion(1.0,2.0,3.0);
	//}
	//else if (arg.key == OIS::KC_F8)
	//{
	//	//this->gameTestThing->startClient();
	//	//mTrayMgr->hideCursor();
	//}
	//else if (arg.key == OIS::KC_F9)
	//{
	//	//this->gameTestThing->startServer();
	//	//mTrayMgr->hideCursor();
	//}
	//else if (arg.key == OIS::KC_F10)
	//{
	//	//printf("%d - %d = %d\n",sizeof(NetTower),420000,sizeof(NetTower)-420000);
	//	//this->gameTestThing->resetTower();
 //   }
 //   else if (arg.key == OIS::KC_F11)
	//{
	//	this->gameTestThing->startLocal();
	//	mTrayMgr->hideCursor();
 //   }
 //   //else if (arg.key == OIS::KC_T)
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

	if(!leaderBoard) {
        this->playerInput.signals.look(ms.X.rel, ms.Y.rel);
	}

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
	Ogre::OverlayManager& overlayManager = Ogre::OverlayManager::getSingleton();
    if(button->getName() == "StartLocalGame")
	{
				this->enginestart->stopAllSounds();
		//startRound();
	}
    else if(button->getName() == "JoinGame")
	{
		this->enginestart->stopAllSounds();
		//this->enginestart->setSoundVolume(0.4f);
				//this->enginestart->play2D("sounds/refactortheme1.mp3");
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
				this->enginestart->stopAllSounds();
				//this->enginestart->setSoundVolume(0.4f);
				//this->enginestart->play2D("sounds/refactortheme1.mp3");
		//this->gameTestThing->destroyScene();
		startRound();
	    this->mRoot->clearEventTimes();
	    this->gameTestThing->destroyScene();
		this->gameTestThing->startServer();
		mTrayMgr->hideCursor();
		mTrayMgr->destroyAllWidgets();
		mTrayMgr->clearAllTrays();
	}
	else if(button->getName() == "Controls")
	{
		if(b1->isVisible())b1->hide();
		if(b2->isVisible())b2->hide();
		if(b3->isVisible())b3->hide();
		if(b4->isVisible())b4->hide();
		if(mTrayMgr->isLogoVisible())mTrayMgr->hideLogo();
		butt->show();
		// Create an overlay, and add the panel
		if(overlayManager.getByName("OverlayName") == NULL)
		{
			overlay = overlayManager.create("OverlayName");
		}
		else
		{
			overlay = overlayManager.getByName("OverlayName");
		}
		overlay->setZOrder(200);
		overlay->add2D(panel21);
		// Show the overlay
		if(!overlay->isVisible()) 
		{
			overlay->show();	
		}
	
	}
	else if(button->getName() == "Exit")
	{
				this->enginestart->stopAllSounds();
		this->gameTestThing->destroyScene();
		mTrayMgr->hideCursor();
		mTrayMgr->destroyAllWidgets();
		mTrayMgr->clearAllTrays();
		mShutDown = true;
	}
	else if(button->getName() == "Back")
	{
		Ogre::OverlayManager& overlayManager = Ogre::OverlayManager::getSingleton();
		if(overlayManager.getByName("OverlayName") != NULL){
			overlay->hide();
			overlay->remove2D(panel21);
		}
		if(butt->isVisible())butt->hide();
		b1->show();
		b2->show();
		b3->show();
		b4->show();
		mTrayMgr->showLogo(OgreBites::TL_TOPLEFT);

	}
	else if(button->getName() == "Back2")
	{
		alreadyshown = FALSE;
		overlayManager.getByName("OverlayName")->clear();
		mTrayMgr->clearAllTrays();
		mTrayMgr->destroyAllWidgets();
		mTrayMgr->createButton(OgreBites::TL_CENTER, "Exit2", "Exit Game");
		mTrayMgr->createButton(OgreBites::TL_CENTER, "Controls2", "Show Controls");
		mTrayMgr->createLabel(OgreBites::TL_CENTER,"info","Press Escape to return to game");
		
		mTrayMgr->showAll();
	}
	else if(button->getName() == "Controls2")
	{
		alreadyshown = TRUE;
		mTrayMgr->clearAllTrays();
		mTrayMgr->destroyAllWidgets();
		//butt->show();
		// Create an overlay, and add the panel
		if(overlayManager.getByName("OverlayName") == NULL)
		{
			overlay2 = overlayManager.create("OverlayName");
		}
		else
		{
			overlay2 = overlayManager.getByName("OverlayName");
		}
		overlay2->setZOrder(200);
		overlay2->add2D(panel31);
		// Show the overlay
		if(!overlay2->isVisible()) 
		{
			overlay2->show();	
		}
		
		mTrayMgr->createButton(OgreBites::TL_BOTTOMRIGHT, "Back2", "Back To Menu");
	
	}
	else if(button->getName() == "Exit2")
	{
		this->gameTestThing->destroyLocalPlayer();
		this->gameTestThing->destroyScene();
		mTrayMgr->clearAllTrays();
		mTrayMgr->destroyAllWidgets();
		overlayManager.destroyAll();
		exit(0);
	}
}

//This would show an overlay with scores on and who has won the game and set the camera up in the sky for a nice frozen view of the end of the game
void Game::endRound()
{
	endofRound = true;
	this->mRoot->clearEventTimes();
	mTrayMgr->clearAllTrays();
	mTrayMgr->destroyAllWidgets();
	//this->gameTestThing->destroyScene();
	this->enginestart->stopAllSounds();
	Ogre::OverlayManager& overlayManager = Ogre::OverlayManager::getSingleton();

	overlayManager.destroyAllOverlayElements();
	//overlayManager.destroyOverlayElement("OverlayName");
	/*overlayManager.destroyOverlayElement("AAAPanelName3");
	overlayManager.destroyOverlayElement("AAAPanelName4");
	overlayManager.destroyOverlayElement("AAAPanelName5");
	overlayManager.destroyOverlayElement("AAAPanelName6");
	overlayManager.destroyOverlayElement("AAAPanelName7");
	overlayManager.destroyOverlayElement("CharGreenPanel");
	overlayManager.destroyOverlayElement("CharOrangePanel");
	overlayManager.destroyOverlayElement("CharPurplePanel");
	overlayManager.destroyOverlayElement("CharRedPanel");
	overlayManager.destroyOverlayElement("PanelName1");
	overlayManager.destroyOverlayElement("PanelName2");
	overlayManager.destroyOverlayElement("PanelName21");
	overlayManager.destroyOverlayElement("PanelName22");
	overlayManager.destroyOverlayElement("greenTeamPoints");
	overlayManager.destroyOverlayElement("greenTeamWins");
	overlayManager.destroyOverlayElement("numberOfWins");
	overlayManager.destroyOverlayElement("orangeTeamPoints");
	overlayManager.destroyOverlayElement("orangeTeamWins");
	overlayManager.destroyOverlayElement("purpleTeamPoints");
	overlayManager.destroyOverlayElement("purpleTeamWins");
	overlayManager.destroyOverlayElement("redTeamPoints");
	overlayManager.destroyOverlayElement("redTeamWins");
	overlayManager.destroyOverlayElement("teamWins");
	overlayManager.destroyOverlayElement("timeHeld");*/

	//overlayManager.getByName("OverlayName")->clear();
	if(overlayManager.getByName("OverlayName") != NULL)
	{
		overlayManager.getByName("OverlayName")->clear();
	}

	Ogre::OverlayContainer* panel3 = static_cast<Ogre::OverlayContainer*>(
    overlayManager.createOverlayElement("Panel", "AAAPanelName3"));
    panel3->setMetricsMode(Ogre::GMM_PIXELS);
    panel3->setPosition(10, 10);
	panel3->setDimensions(mWindow->getWidth() - 20, (mWindow->getHeight()/7));
	Ogre::MaterialPtr crosshair3 = Ogre::MaterialManager::getSingleton().create("crosshair3", "General");
    crosshair3->getTechnique(0)->getPass(0)->createTextureUnitState("shadow.png");
    crosshair3->getTechnique(0)->getPass(0)->setSceneBlending(Ogre::SBT_TRANSPARENT_ALPHA);
    panel3->setMaterialName("crosshair3");

	Ogre::OverlayContainer* panel4 = static_cast<Ogre::OverlayContainer*>(
    overlayManager.createOverlayElement("Panel", "AAAPanelName4"));
    panel4->setMetricsMode(Ogre::GMM_PIXELS);
    panel4->setPosition(10, (mWindow->getHeight()/7) + 20);
	panel4->setDimensions((mWindow->getWidth()/4) - 20, ((mWindow->getHeight()/7)*6) - 30);
	Ogre::MaterialPtr crosshair4 = Ogre::MaterialManager::getSingleton().create("crosshair4", "General");
    crosshair4->getTechnique(0)->getPass(0)->createTextureUnitState("shadow.png");
    crosshair4->getTechnique(0)->getPass(0)->setSceneBlending(Ogre::SBT_TRANSPARENT_ALPHA);
    panel4->setMaterialName("crosshair4");

	Ogre::OverlayContainer* panel5 = static_cast<Ogre::OverlayContainer*>(
    overlayManager.createOverlayElement("Panel", "AAAPanelName5"));
    panel5->setMetricsMode(Ogre::GMM_PIXELS);
    panel5->setPosition((mWindow->getWidth()/4) + 10, (mWindow->getHeight()/7) + 20);
	panel5->setDimensions((mWindow->getWidth()/4) - 20, ((mWindow->getHeight()/7)*6) - 30);
	Ogre::MaterialPtr crosshair5 = Ogre::MaterialManager::getSingleton().create("crosshair5", "General");
    crosshair5->getTechnique(0)->getPass(0)->createTextureUnitState("shadow.png");
    crosshair5->getTechnique(0)->getPass(0)->setSceneBlending(Ogre::SBT_TRANSPARENT_ALPHA);
    panel5->setMaterialName("crosshair5");
	
	Ogre::OverlayContainer* panel6 = static_cast<Ogre::OverlayContainer*>(
    overlayManager.createOverlayElement("Panel", "AAAPanelName6"));
    panel6->setMetricsMode(Ogre::GMM_PIXELS);
    panel6->setPosition((mWindow->getWidth()/4)*2 + 10, (mWindow->getHeight()/7) + 20);
	panel6->setDimensions((mWindow->getWidth()/4) - 20, ((mWindow->getHeight()/7)*6) - 30);
	Ogre::MaterialPtr crosshair6 = Ogre::MaterialManager::getSingleton().create("crosshair6", "General");
    crosshair6->getTechnique(0)->getPass(0)->createTextureUnitState("shadow.png");
    crosshair6->getTechnique(0)->getPass(0)->setSceneBlending(Ogre::SBT_TRANSPARENT_ALPHA);
    panel6->setMaterialName("crosshair6");
	
	Ogre::OverlayContainer* panel7 = static_cast<Ogre::OverlayContainer*>(
    overlayManager.createOverlayElement("Panel", "AAAPanelName7"));
    panel7->setMetricsMode(Ogre::GMM_PIXELS);
    panel7->setPosition((mWindow->getWidth()/4)*3 + 10, (mWindow->getHeight()/7) + 20);
	panel7->setDimensions((mWindow->getWidth()/4) - 20, ((mWindow->getHeight()/7)*6) - 30);
	Ogre::MaterialPtr crosshair7 = Ogre::MaterialManager::getSingleton().create("crosshair7", "General");
    crosshair7->getTechnique(0)->getPass(0)->createTextureUnitState("shadow.png");
    crosshair7->getTechnique(0)->getPass(0)->setSceneBlending(Ogre::SBT_TRANSPARENT_ALPHA);
    panel7->setMaterialName("crosshair7");

	float width = (mWindow->getWidth()/4) - 20;
	float height = width * 1.130769;
	Ogre::OverlayContainer* panel8 = static_cast<Ogre::OverlayContainer*>(
    overlayManager.createOverlayElement("Panel", "CharRedPanel"));
    panel8->setMetricsMode(Ogre::GMM_PIXELS);
    panel8->setPosition(10, (mWindow->getHeight()/7) + 25);
	panel8->setDimensions(width, height);
	Ogre::MaterialPtr crosshair8 = Ogre::MaterialManager::getSingleton().create("redChar", "General");
    crosshair8->getTechnique(0)->getPass(0)->createTextureUnitState("redChar.png");
    crosshair8->getTechnique(0)->getPass(0)->setSceneBlending(Ogre::SBT_TRANSPARENT_ALPHA);
    panel8->setMaterialName("redChar");

	Ogre::OverlayContainer* panel9 = static_cast<Ogre::OverlayContainer*>(
    overlayManager.createOverlayElement("Panel", "CharOrangePanel"));
    panel9->setMetricsMode(Ogre::GMM_PIXELS);
    panel9->setPosition((mWindow->getWidth()/4) + 10, (mWindow->getHeight()/7) + 25);
	panel9->setDimensions(width, height);
	Ogre::MaterialPtr crosshair9 = Ogre::MaterialManager::getSingleton().create("orangeChar", "General");
    crosshair9->getTechnique(0)->getPass(0)->createTextureUnitState("orangeChar.png");
    crosshair9->getTechnique(0)->getPass(0)->setSceneBlending(Ogre::SBT_TRANSPARENT_ALPHA);
    panel9->setMaterialName("orangeChar");
	
	Ogre::OverlayContainer* panel10 = static_cast<Ogre::OverlayContainer*>(
    overlayManager.createOverlayElement("Panel", "CharPurplePanel"));
    panel10->setMetricsMode(Ogre::GMM_PIXELS);
    panel10->setPosition((mWindow->getWidth()/4)*2 + 10, (mWindow->getHeight()/7) + 25);
	panel10->setDimensions(width, height);
	Ogre::MaterialPtr crosshair10 = Ogre::MaterialManager::getSingleton().create("purpleChar", "General");
    crosshair10->getTechnique(0)->getPass(0)->createTextureUnitState("purpleChar.png");
    crosshair10->getTechnique(0)->getPass(0)->setSceneBlending(Ogre::SBT_TRANSPARENT_ALPHA);
    panel10->setMaterialName("purpleChar");
	
	Ogre::OverlayContainer* panel11 = static_cast<Ogre::OverlayContainer*>(
    overlayManager.createOverlayElement("Panel", "CharGreenPanel"));
    panel11->setMetricsMode(Ogre::GMM_PIXELS);
    panel11->setPosition((mWindow->getWidth()/4)*3 + 10, (mWindow->getHeight()/7) + 25);
	panel11->setDimensions(width, height);
	Ogre::MaterialPtr crosshair11 = Ogre::MaterialManager::getSingleton().create("greenChar", "General");
    crosshair11->getTechnique(0)->getPass(0)->createTextureUnitState("greenChar.png");
    crosshair11->getTechnique(0)->getPass(0)->setSceneBlending(Ogre::SBT_TRANSPARENT_ALPHA);
    panel11->setMaterialName("greenChar");
    
    // Create a panel
    Ogre::OverlayContainer* panel2 = static_cast<Ogre::OverlayContainer*>(
    overlayManager.createOverlayElement("Panel", "PanelName2"));
    panel2->setMetricsMode(Ogre::GMM_PIXELS);
    panel2->setPosition(0, 0);
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
    mFont->setSource("Orbitron Bold.ttf");
    mFont->setTrueTypeSize(32);
    mFont->setTrueTypeResolution(110);
	mFont->load();

	/*Ogre::OverlayElement* teamWins = overlayManager.createOverlayElement("TextArea", "teamWins");
    teamWins->setMetricsMode(Ogre::GMM_PIXELS);
    teamWins->setPosition(0, 0);
    teamWins->setDimensions(mWindow->getWidth(), (mWindow->getHeight()/4));
	teamWins->setParameter("char_height", "55");
	teamWins->setParameter("font_name", "MyFont");
    teamWins->setCaption(this->gameTestThing->winningTeam + " TEAM WINS");
    teamWins->setColour(Ogre::ColourValue(0, 0.984, 0.925));*/
	Ogre::TextAreaOverlayElement* teamWins = static_cast<Ogre::TextAreaOverlayElement*>(overlayManager.createOverlayElement("TextArea", "teamWins"));
    teamWins->setMetricsMode(Ogre::GMM_PIXELS);
    teamWins->setPosition(0 + (mWindow->getWidth()/2), 25);
	teamWins->setAlignment(Ogre::TextAreaOverlayElement::Alignment::Center);
    teamWins->setDimensions(mWindow->getWidth(), (mWindow->getHeight()/4));
	teamWins->setParameter("char_height", "55");
	teamWins->setParameter("font_name", "MyFont");
    teamWins->setCaption(this->gameTestThing->winningTeam + " TEAM WINS");
    teamWins->setColour(Ogre::ColourValue(0, 0.984, 0.925));

	if(this->gameTestThing->winningTeam == "RED") this->gameTestThing->wins[0]++;
	else if(this->gameTestThing->winningTeam == "ORANGE") this->gameTestThing->wins[1]++;
	else if(this->gameTestThing->winningTeam == "PURPLE") this->gameTestThing->wins[2]++;
	else if(this->gameTestThing->winningTeam == "GREEN") this->gameTestThing->wins[3]++;

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	int time;
	int minutes;
	int seconds;
	height = (mWindow->getHeight()/7) + 25 + height + 10;
	Ogre::TextAreaOverlayElement* timeHeld = static_cast<Ogre::TextAreaOverlayElement*>(overlayManager.createOverlayElement("TextArea", "timeHeld"));
    timeHeld->setMetricsMode(Ogre::GMM_PIXELS);
    timeHeld->setPosition((mWindow->getWidth()/2), height);
	timeHeld->setAlignment(Ogre::TextAreaOverlayElement::Alignment::Center);
    timeHeld->setDimensions(mWindow->getWidth(), (mWindow->getHeight()/7));
	timeHeld->setParameter("char_height", "18");
	timeHeld->setParameter("font_name", "MyFont");
    timeHeld->setCaption("TIME SPENT IN GOAL");
    timeHeld->setColour(Ogre::ColourValue(0, 0.984, 0.925));

	height = height + 30;
	Ogre::TextAreaOverlayElement* redTeamPoints = static_cast<Ogre::TextAreaOverlayElement*>(overlayManager.createOverlayElement("TextArea", "redTeamPoints"));
    redTeamPoints->setMetricsMode(Ogre::GMM_PIXELS);
    redTeamPoints->setPosition(0 + ((mWindow->getWidth()/4)/2), height);
	redTeamPoints->setAlignment(Ogre::TextAreaOverlayElement::Alignment::Center);
    redTeamPoints->setDimensions((mWindow->getWidth()/4), (mWindow->getHeight()/4));
	redTeamPoints->setParameter("char_height", "26");
	redTeamPoints->setParameter("font_name", "MyFont");
	time = this->gameTestThing->network->teamScores[0];
	if(this->gameTestThing->winningTeam == "RED") time = time - 2;
	time = time/(double)100;
	minutes = floor(time/(double)60);
	seconds = time % 60;
	//printf("%i\n", this->gameTestThing->network->teamScores[0]);
    redTeamPoints->setCaption(Ogre::StringConverter::toString(minutes) + "m " + Ogre::StringConverter::toString(seconds) + "s");
    redTeamPoints->setColour(Ogre::ColourValue(0, 0.984, 0.925));

	Ogre::TextAreaOverlayElement* orangeTeamPoints = static_cast<Ogre::TextAreaOverlayElement*>(overlayManager.createOverlayElement("TextArea", "orangeTeamPoints"));
    orangeTeamPoints->setMetricsMode(Ogre::GMM_PIXELS);
    orangeTeamPoints->setPosition((mWindow->getWidth()/4) + ((mWindow->getWidth()/4)/2), height);
	orangeTeamPoints->setAlignment(Ogre::TextAreaOverlayElement::Alignment::Center);
    orangeTeamPoints->setDimensions((mWindow->getWidth()/4), (mWindow->getHeight()/4));
	orangeTeamPoints->setParameter("char_height", "26");
	orangeTeamPoints->setParameter("font_name", "MyFont");
	time = this->gameTestThing->network->teamScores[1];
	if(this->gameTestThing->winningTeam == "ORANGE") time = time - 2;
	time = time/(double)100;
	minutes = floor(time/(double)60);
	seconds = time % 60;
	//printf("%i\n", this->gameTestThing->network->teamScores[1]);
    orangeTeamPoints->setCaption(Ogre::StringConverter::toString(minutes) + "m " + Ogre::StringConverter::toString(seconds) + "s");
    orangeTeamPoints->setColour(Ogre::ColourValue(0, 0.984, 0.925));

	Ogre::TextAreaOverlayElement* purpleTeamPoints = static_cast<Ogre::TextAreaOverlayElement*>(overlayManager.createOverlayElement("TextArea", "purpleTeamPoints"));
    purpleTeamPoints->setMetricsMode(Ogre::GMM_PIXELS);
    purpleTeamPoints->setPosition((mWindow->getWidth()/4)*2 + ((mWindow->getWidth()/4)/2), height);
	purpleTeamPoints->setAlignment(Ogre::TextAreaOverlayElement::Alignment::Center);
    purpleTeamPoints->setDimensions((mWindow->getWidth()/4), (mWindow->getHeight()/4));
	purpleTeamPoints->setParameter("char_height", "26");
	purpleTeamPoints->setParameter("font_name", "MyFont");
	time = this->gameTestThing->network->teamScores[2];
	if(this->gameTestThing->winningTeam == "PURPLE") time = time - 2;
	time = time/(double)100;
	minutes = floor(time/(double)60);
	seconds = time % 60;
	//printf("%i\n", this->gameTestThing->network->teamScores[2]);
    purpleTeamPoints->setCaption(Ogre::StringConverter::toString(minutes) + "m " + Ogre::StringConverter::toString(seconds) + "s");
    purpleTeamPoints->setColour(Ogre::ColourValue(0, 0.984, 0.925));

	Ogre::TextAreaOverlayElement* greenTeamPoints = static_cast<Ogre::TextAreaOverlayElement*>(overlayManager.createOverlayElement("TextArea", "greenTeamPoints"));
    greenTeamPoints->setMetricsMode(Ogre::GMM_PIXELS);
    greenTeamPoints->setPosition((mWindow->getWidth()/4)*3 + ((mWindow->getWidth()/4)/2), height);
	greenTeamPoints->setAlignment(Ogre::TextAreaOverlayElement::Alignment::Center);
    greenTeamPoints->setDimensions((mWindow->getWidth()/4), (mWindow->getHeight()/4));
	greenTeamPoints->setParameter("char_height", "26");
	greenTeamPoints->setParameter("font_name", "MyFont");
	time = this->gameTestThing->network->teamScores[3];
	if(this->gameTestThing->winningTeam == "GREEN") time = time - 2;
	time = time/(double)100;
	minutes = floor(time/(double)60);
	seconds = time % 60;
	//printf("%i\n", this->gameTestThing->network->teamScores[3]);
    greenTeamPoints->setCaption(Ogre::StringConverter::toString(minutes) + "m " + Ogre::StringConverter::toString(seconds) + "s");
    greenTeamPoints->setColour(Ogre::ColourValue(0, 0.984, 0.925));

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	height = (mWindow->getHeight()/4)*3 - 30;
	Ogre::TextAreaOverlayElement* numberOfWins = static_cast<Ogre::TextAreaOverlayElement*>(overlayManager.createOverlayElement("TextArea", "numberOfWins"));
    numberOfWins->setMetricsMode(Ogre::GMM_PIXELS);
    numberOfWins->setPosition((mWindow->getWidth()/2), height);
	numberOfWins->setAlignment(Ogre::TextAreaOverlayElement::Alignment::Center);
    numberOfWins->setDimensions(mWindow->getWidth(), (mWindow->getHeight()/7));
	numberOfWins->setParameter("char_height", "18");
	numberOfWins->setParameter("font_name", "MyFont");
    numberOfWins->setCaption("NUMBER OF WINS");
    numberOfWins->setColour(Ogre::ColourValue(0, 0.984, 0.925));

	Ogre::TextAreaOverlayElement* redTeamWins = static_cast<Ogre::TextAreaOverlayElement*>(overlayManager.createOverlayElement("TextArea", "redTeamWins"));
    redTeamWins->setMetricsMode(Ogre::GMM_PIXELS);
    redTeamWins->setPosition(0 + ((mWindow->getWidth()/4)/2), (mWindow->getHeight()/4)*3);
	redTeamWins->setAlignment(Ogre::TextAreaOverlayElement::Alignment::Center);
    redTeamWins->setDimensions((mWindow->getWidth()/4), (mWindow->getHeight()/4));
	redTeamWins->setParameter("char_height", "50");
	redTeamWins->setParameter("font_name", "MyFont");
    redTeamWins->setCaption(Ogre::StringConverter::toString(this->gameTestThing->wins[0]));
    redTeamWins->setColour(Ogre::ColourValue(0, 0.984, 0.925));

	Ogre::TextAreaOverlayElement* orangeTeamWins = static_cast<Ogre::TextAreaOverlayElement*>(overlayManager.createOverlayElement("TextArea", "orangeTeamWins"));
    orangeTeamWins->setMetricsMode(Ogre::GMM_PIXELS);
    orangeTeamWins->setPosition((mWindow->getWidth()/4) + ((mWindow->getWidth()/4)/2), (mWindow->getHeight()/4)*3);
	orangeTeamWins->setAlignment(Ogre::TextAreaOverlayElement::Alignment::Center);
    orangeTeamWins->setDimensions((mWindow->getWidth()/4), (mWindow->getHeight()/4));
	orangeTeamWins->setParameter("char_height", "50");
	orangeTeamWins->setParameter("font_name", "MyFont");
    orangeTeamWins->setCaption(Ogre::StringConverter::toString(this->gameTestThing->wins[1]));
    orangeTeamWins->setColour(Ogre::ColourValue(0, 0.984, 0.925));

	Ogre::TextAreaOverlayElement* purpleTeamWins = static_cast<Ogre::TextAreaOverlayElement*>(overlayManager.createOverlayElement("TextArea", "purpleTeamWins"));
    purpleTeamWins->setMetricsMode(Ogre::GMM_PIXELS);
    purpleTeamWins->setPosition((mWindow->getWidth()/4)*2 + ((mWindow->getWidth()/4)/2), (mWindow->getHeight()/4)*3);
	purpleTeamWins->setAlignment(Ogre::TextAreaOverlayElement::Alignment::Center);
    purpleTeamWins->setDimensions((mWindow->getWidth()/4), (mWindow->getHeight()/4));
	purpleTeamWins->setParameter("char_height", "50");
	purpleTeamWins->setParameter("font_name", "MyFont");
    purpleTeamWins->setCaption(Ogre::StringConverter::toString(this->gameTestThing->wins[2]));
    purpleTeamWins->setColour(Ogre::ColourValue(0, 0.984, 0.925));

	Ogre::TextAreaOverlayElement* greenTeamWins = static_cast<Ogre::TextAreaOverlayElement*>(overlayManager.createOverlayElement("TextArea", "greenTeamWins"));
    greenTeamWins->setMetricsMode(Ogre::GMM_PIXELS);
    greenTeamWins->setPosition((mWindow->getWidth()/4)*3 + ((mWindow->getWidth()/4)/2), (mWindow->getHeight()/4)*3);
	greenTeamWins->setAlignment(Ogre::TextAreaOverlayElement::Alignment::Center);
    greenTeamWins->setDimensions((mWindow->getWidth()/4), (mWindow->getHeight()/4));
	greenTeamWins->setParameter("char_height", "50");
	greenTeamWins->setParameter("font_name", "MyFont");
    greenTeamWins->setCaption(Ogre::StringConverter::toString(this->gameTestThing->wins[3]));
    greenTeamWins->setColour(Ogre::ColourValue(0, 0.984, 0.925));
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//Ogre::MaterialPtr crosshair2 = Ogre::MaterialManager::getSingleton().create("crosshair2", "General");
   // crosshair2->getTechnique(0)->getPass(0)->createTextureUnitState("grass_1024.jpg");
    //crosshair2->getTechnique(0)->getPass(0)->setDepthCheckEnabled(true);
   // crosshair2->getTechnique(0)->getPass(0)->setSceneBlending(Ogre::SBT_TRANSPARENT_ALPHA);
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
	panel2->addChild(panel3);
	panel2->addChild(panel4);
	panel2->addChild(panel5);
	panel2->addChild(panel6);
	panel2->addChild(panel7);
	panel2->addChild(panel8);
	panel2->addChild(panel9);
	panel2->addChild(panel10);
	panel2->addChild(panel11);
	panel2->addChild(timeHeld);
	panel2->addChild(numberOfWins);

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
	endofRound = false;
	//setUpPhysicsWorld();
	//this->gameTestThing->startLocal();
	if(mTrayMgr->isCursorVisible()) {
	    mTrayMgr->hideCursor();
	}
    mTrayMgr->destroyAllWidgets();
	mTrayMgr->clearAllTrays();

	Ogre::OverlayManager& overlayManager = Ogre::OverlayManager::getSingleton();

	//overlayManager.destroyAllOverlayElements();
	if(overlayManager.getByName("OverlayName") != NULL)
	{
		overlayManager.getByName("OverlayName")->clear();
	}
    // Create a panel
    panel = static_cast<Ogre::OverlayContainer*>(
    overlayManager.createOverlayElement("Panel", "PanelName1"));
    panel->setMetricsMode(Ogre::GMM_PIXELS);
    panel->setPosition((mWindow->getWidth()/2)-10, (mWindow->getHeight()/2)-10);
	panel->setDimensions(20,20);
    Ogre::MaterialPtr crosshair = Ogre::MaterialManager::getSingleton().create("crosshair2", "General");
    crosshair->getTechnique(0)->getPass(0)->createTextureUnitState("crosshairbrackets.png");
    //crosshair->getTechnique(0)->getPass(0)->setDepthCheckEnabled(true);
    crosshair->getTechnique(0)->getPass(0)->setSceneBlending(Ogre::SBT_TRANSPARENT_ALPHA);
    panel->setMaterialName("crosshair2");
    
    // Create an overlay, and add the panel
	Ogre::Overlay* overlay2 = NULL;
	if(overlayManager.getByName("OverlayName") == NULL)
	{
        overlay2 = overlayManager.create("OverlayName");
	}
	else
	{
		overlay2 = overlayManager.getByName("OverlayName");
	}
    overlay2->add2D(panel);
    
    // Show the overlay
	if(!overlay2->isVisible()) 
	{
        overlay2->show();
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