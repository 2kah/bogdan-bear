#include "GameTestThing.h"

#include <iostream>
#include <string>
#include <vector>

#include <boost/signal.hpp>
#include <boost/bind.hpp>

#include <OGRE/OgreVector3.h>

#include <btBulletWorldImporter.h>
#include <OGRE/OgreOverlay.h>
#include <OGRE/OgreOverlayManager.h>

#include "Game.h"
#include "NetworkTestStuff.h"

#include "Sounds.h"

#include "Tower.h"
#include "TowerPhysics.h"
#include "TowerGraphics.h"
#include "TowerBuilder.h"

#include "Turret.h"
#include "TurretGraphics.h"

#include "Player.h"
#include "PlayerPhysics.h"
#include "PlayerGraphics.h"
#include "PlayerCamera.h"
#include "PlayerProperties.h"
#include "PlayerSound.h"

#include "Platform.h"
#include "PlatformGraphics.h"
#include "PlatformPhysics.h"

#include "Rocket.h"
#include "RocketGraphics.h"
#include "RocketPhysics.h"
#include "RocketSound.h"
#include "GlowMaterialListener.h"

#include "Explosion.h"
#include "ExplosionPhysics.h"
#include "ExplosionGraphics.h"

#include "Goal.h"

#include "FallingObject.h"

bool isLocal = false;
bool isServer = false;

GameTestThing::GameTestThing(Game *game)
    : game(game)
    , localPlayer(NULL)
	, goal(NULL)
{
    this->network = new NetworkTestStuff();
	this->network->game_obj = this->game;
    this->network->signals.chat.connect(boost::bind(&GameTestThing::chatReceived, this, _1));
    this->network->signals.explosion.connect(boost::bind(&GameTestThing::networkExplosion, this, _1, _2, _3, _4));
	this->network->signals.recvRocket.connect(boost::bind(&GameTestThing::networkRocket, this, _1, _2));
    //this->game->objects.insert(this->network);

	this->network->signals.addPlayer.connect(boost::bind(&GameTestThing::addPlayer, this, _1, _2));
	this->network->signals.removePlayer.connect(boost::bind(&GameTestThing::removePlayer, this, _1));
	this->network->signals.assignLocalPlayer.connect(boost::bind(&GameTestThing::setLocalPlayer, this, _1));
    this->sounds = new Sounds();

	//TODO: remove this - needed because networking is hacky
	this->towerBuilder = NULL;

    // Create and add the ground plane
    btCollisionShape *groundShape = new btStaticPlaneShape(btVector3(0, 1, 0), 1);
    
	btDefaultMotionState* groundMotionState = new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1),btVector3(0,-1,0)));
    btRigidBody::btRigidBodyConstructionInfo
                groundRigidBodyCI(0,groundMotionState,groundShape,btVector3(0,0,0));
    btRigidBody* groundRigidBody = new btRigidBody(groundRigidBodyCI);
    this->game->dynamicsWorld->addRigidBody(groundRigidBody); // , 4, 2);
    
    // Add the bowl
	Ogre::CompositorManager::getSingleton().addCompositor(this->game->mCamera->getViewport(), "Glow");
	Ogre::CompositorManager::getSingleton().setCompositorEnabled(this->game->mCamera->getViewport(), "Glow", true);
	GlowMaterialListener *gml = new GlowMaterialListener();
	Ogre::MaterialManager::getSingleton().addListener(gml);

    Ogre::Entity *bowl = this->game->mSceneMgr->createEntity("Bowlchip.mesh");
    Ogre::SceneNode *sceneNode = this->game->mSceneMgr->getRootSceneNode()->createChildSceneNode();
    sceneNode->attachObject(bowl);
    sceneNode->setScale(30*Ogre::Vector3::UNIT_SCALE);

    btBulletWorldImporter* fileLoader = new btBulletWorldImporter(this->game->dynamicsWorld);
	fileLoader->loadFile("BowlBul.bullet");

    // Create and add a falling object
    /*FallingObject *object = new FallingObject(Ogre::Vector3(40.5, 64, 40.15));
    object->addToScene(this->game->mSceneMgr);
    object->addToPhysics(this->game->dynamicsWorld);
    this->game->objects.insert(object);*/

	//Fog for gradient
	Ogre::ColourValue fadeColour(0.005, 0.005, 0.005);
    //this->game->mSceneMgr->setFog(Ogre::FOG_LINEAR, fadeColour, 0.0, 10, 1200);
    //this->game->mWindow->getViewport(0)->setBackgroundColour(fadeColour);
	//Loads up nebula sky
	this->game->mSceneMgr->setSkyBox(true, "nebulaBox", 700, true);
	//Ambient light
	this->game->mSceneMgr->setAmbientLight(Ogre::ColourValue::White);

	this->game->mSceneMgr->setAmbientLight(Ogre::ColourValue::White);
	//Ogre::Light *spot2 = this->game->mSceneMgr->createLight();
	//spot2->setType(Ogre::Light::LT_POINT);
	//spot2->setPosition(0,200,0);
    //spot2->setDiffuseColour(0,255,246);
    //spot2->setSpecularColour(Ogre::ColourValue::White);
	Ogre::Light *moon = this->game->mSceneMgr->createLight();
    moon->setType(Ogre::Light::LT_DIRECTIONAL);
    moon->setDirection(Ogre::Vector3(-1, -1, -1));
    moon->setDiffuseColour(Ogre::ColourValue::White);
	//moon->setDiffuseColour(Ogre::ColourValue(100, 100, 100));

    Ogre::Light *moon2 = this->game->mSceneMgr->createLight();
    moon2->setType(Ogre::Light::LT_DIRECTIONAL);
    moon2->setDirection(Ogre::Vector3(1, 1, 1));
    moon2->setDiffuseColour(Ogre::ColourValue::White);
	//moon2->setDiffuseColour(Ogre::ColourValue(100, 100, 100));

	/*Ogre::Light *moon3 = this->game->mSceneMgr->createLight();
    moon3->setType(Ogre::Light::LT_DIRECTIONAL);
    moon3->setDirection(Ogre::Vector3(0, 0, 1));
    moon3->setDiffuseColour(Ogre::ColourValue::White);
	//moon3->setDiffuseColour(Ogre::ColourValue(100, 100, 100));

	Ogre::Light *moon4 = this->game->mSceneMgr->createLight();
    moon4->setType(Ogre::Light::LT_DIRECTIONAL);
    moon4->setDirection(Ogre::Vector3(0, 0, -1));
    moon4->setDiffuseColour(Ogre::ColourValue::White);
	//moon4->setDiffuseColour(Ogre::ColourValue(100, 100, 100));

	Ogre::Light *moon5 = this->game->mSceneMgr->createLight();
    moon5->setType(Ogre::Light::LT_DIRECTIONAL);
    moon5->setDirection(Ogre::Vector3(0, -1, 0));
    //moon5->setDiffuseColour(Ogre::ColourValue(0, 255, 246));
    moon5->setDiffuseColour(Ogre::ColourValue::White);
	//moon5->setDiffuseColour(Ogre::ColourValue(100, 100, 100));*/

    std::cout << "PRESS F11 FOR LOCAL TEST GAME" << std::endl;
    std::cout << "PRESS F9 FOR LOCAL SERVER GAME" << std::endl;
    std::cout << "PRESS F8 FOR LOCAL CLIENT GAME" << std::endl;

}

GameTestThing::~GameTestThing()
{
}

void GameTestThing::resetTower()
{
}

void GameTestThing::buildScene()
{
	isLocal = true;
	isServer = false;
    // Create an empty tower
    unsigned divisions[] = {8, 16, 16, 32, 32, 32, 64, 64, 64, 64, 64, 64, 64, 64, 64, 128, 128, 128, 128, 128, 128, 128};
    std::vector<unsigned> structure(divisions, divisions + 14 + 8);
    //unsigned divisions[] = {32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32};
    //std::vector<unsigned> structure(divisions, divisions + 14);

    this->game->tower = new Tower(256, structure);

    // Create a tower builder and generate the tower with it
    this->towerBuilder = new TowerBuilder(this->game->tower);
    this->towerBuilder->generate();

    // Add the tower builder to the set of things to update
    //this->game->objects.insert(builder);

    // Add four turrets
    Turret *turret1 = new Turret(Ogre::Vector3(0, 130, 400), Ogre::Quaternion(Ogre::Degree(-180), Ogre::Vector3::UNIT_Y));
	Turret *turret2 = new Turret(Ogre::Vector3(0, 130, -400), Ogre::Quaternion(Ogre::Degree(-0), Ogre::Vector3::UNIT_Y));
	Turret *turret3 = new Turret(Ogre::Vector3(400, 130, 0), Ogre::Quaternion(Ogre::Degree(-90), Ogre::Vector3::UNIT_Y));
	Turret *turret4 = new Turret(Ogre::Vector3(-400, 130, 0), Ogre::Quaternion(Ogre::Degree(-270), Ogre::Vector3::UNIT_Y));

    this->addTurret(turret1);
    this->addTurret(turret2);
    this->addTurret(turret3);
    this->addTurret(turret4);

    // Add tower graphics and physics
    new TowerGraphics(this->game->tower, this->game->mSceneMgr);
    new TowerPhysics(this->game->tower, this->game->dynamicsWorld);

	Ogre::Entity *bowl = this->game->mSceneMgr->createEntity("Bowlchip.mesh");
    Ogre::SceneNode *sceneNode = this->game->mSceneMgr->getRootSceneNode()->createChildSceneNode();
    sceneNode->attachObject(bowl);
    sceneNode->setScale(30*Ogre::Vector3::UNIT_SCALE);

    btBulletWorldImporter* fileLoader = new btBulletWorldImporter(this->game->dynamicsWorld);
	fileLoader->loadFile("BowlBul.bullet");

}

//removes the player and physics so a new one can be made. clears the objects so the old ones don't get updated, re adds the network, destroys old graphics (except tower). need to delete tower.
void GameTestThing::destroyScene()
{
	//this->game->tower = NULL;
	//this->towerBuilder = NULL;
	if(this->localPlayer != NULL)
	{
		this->localPlayerPhysics->~PlayerPhysics();
		delete this->localPlayer;
	}
	this->game->objects.clear();
	this->game->objects.insert(this->network);
	this->removeQueue.clear();
	Turret *turret;
	for(std::set<Turret *>::iterator i = this->turrets.begin(); i != this->turrets.end(); ++i)
    {
        turret = *i;
		delete turret;
    }
	this->turrets.clear();
	//delete this->towerBuilder;
	this->game->mSceneMgr->getRootSceneNode()->removeAndDestroyAllChildren();
	this->game->mSceneMgr->destroyAllEntities();
	//this->network->tb = NULL;
	//this->towerBuilder = NULL;
	//delete this->towerBuilder;
	//this->game->tower->~Tower();
	//delete this->game->tower;
	//this->game->mSceneMgr->clearSpecialCaseRenderQueues();
	//this->game->mSceneMgr->clearScene();
	this->game->mSceneMgr->destroyAllManualObjects();
	for (int i=this->game->dynamicsWorld->getNumCollisionObjects()-1; i>=0 ;i--)
        {
                btCollisionObject* obj = this->game->dynamicsWorld->getCollisionObjectArray()[i];
                this->game->dynamicsWorld->removeCollisionObject( obj );
                delete obj;
        }
	//this->game->mSceneMgr->destroyAllManualObjects();
	//this->game->dynamicsWorld->getBroadphase()->~btBroadphaseInterface();
	//delete this->game->dynamicsWorld;
	//this->game->setUpPhysicsWorld();
	//this->game->mSceneMgr->clearScene();
}

void GameTestThing::startLocal()
{
	isLocal = true;
	isServer = false;
    // Create an empty tower
	//TODO: make tower wider
	//was {8, 16, 16, 32, 32, 32, 64, 64, 64, 64, 64, 64, 64, 64, 64, 128, 128, 128, 128, 128, 128, 128}
    unsigned divisions[] = {8, 16, 16, 32, 32, 32, 64, 64, 64, 64, 64, 64, 64, 64, 64, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 256, 256, 256, 256, 256, 256, 256};
    std::vector<unsigned> structure(divisions, divisions + 14 + 8 + 11);
    //unsigned divisions[] = {32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32};
    //std::vector<unsigned> structure(divisions, divisions + 14);

    this->game->tower = new Tower(192, structure);

    // Create a tower builder and generate the tower with it
    this->towerBuilder = new TowerBuilder(this->game->tower);
	this->network->tb = this->towerBuilder;
	this->towerBuilder->isPaused=false;
	//this->game->tower->synchronise();
	this->towerBuilder->generate();

    // Add the tower builder to the set of things to update
    //this->game->objects.insert(builder);

    // Add four turrets
    Turret *turret1 = new Turret(Ogre::Vector3(0, 130, 400), Ogre::Quaternion(Ogre::Degree(-180), Ogre::Vector3::UNIT_Y));
	Turret *turret2 = new Turret(Ogre::Vector3(0, 130, -400), Ogre::Quaternion(Ogre::Degree(-0), Ogre::Vector3::UNIT_Y));
	Turret *turret3 = new Turret(Ogre::Vector3(400, 130, 0), Ogre::Quaternion(Ogre::Degree(-90), Ogre::Vector3::UNIT_Y));
	Turret *turret4 = new Turret(Ogre::Vector3(-400, 130, 0), Ogre::Quaternion(Ogre::Degree(-270), Ogre::Vector3::UNIT_Y));

    this->addTurret(turret1);
    this->addTurret(turret2);
    this->addTurret(turret3);
    this->addTurret(turret4);

    // Add tower graphics and physics
    new TowerGraphics(this->game->tower, this->game->mSceneMgr);
    new TowerPhysics(this->game->tower, this->game->dynamicsWorld);
	
    // Add a player
    Player *player = new Player(Ogre::Vector3(0, this->game->tower->levels * this->game->tower->block_height + 10, 10));
	Player *enemy =  new Player(Ogre::Vector3(100, 20, 100));
	Player *enemy1 = new Player(Ogre::Vector3(85, 20, 100));
	Player *enemy2 = new Player(Ogre::Vector3(70, 20, 100));
	Player *enemy3 = new Player(Ogre::Vector3(55, 20, 100));

    this->addPlayer(player, 0);
    this->setLocalPlayer(player);
	
    this->addPlayer(enemy, 0);
	this->addPlayer(enemy1, 1);
	this->addPlayer(enemy2, 2);
	this->addPlayer(enemy3, 3);

	//Add the goal
	Goal *goal = new Goal(Ogre::Vector3(0, this->game->tower->levels * this->game->tower->block_height, 0), player, this->game->mSceneMgr, this->game->dynamicsWorld);
	this->game->objects.insert(goal);
	this->goal = goal;
    
	
	Ogre::Entity *bowl = this->game->mSceneMgr->createEntity("Bowlchip.mesh");
    Ogre::SceneNode *sceneNode = this->game->mSceneMgr->getRootSceneNode()->createChildSceneNode();
    sceneNode->attachObject(bowl);
    sceneNode->setScale(30*Ogre::Vector3::UNIT_SCALE);

    btBulletWorldImporter* fileLoader = new btBulletWorldImporter(this->game->dynamicsWorld);
	fileLoader->loadFile("BowlBul.bullet");

    // Set the turret to aim at the player always. Setting it to NULL makes it shoot randomly at the tower.
	//turret1->setTarget(player);
	//turret2->setTarget(player);
	//turret3->setTarget(player);
	//turret4->setTarget(player);
}

void GameTestThing::startClient()
{
	isLocal = false;
	isServer = false;

	unsigned divisions[] = {8, 16, 16, 32, 32, 32, 64, 64, 64, 64, 64, 64, 64, 64, 64, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 256, 256, 256, 256, 256, 256, 256};
    std::vector<unsigned> structure(divisions, divisions + 14 + 8 + 11);

	/*unsigned divisions[] = {8, 16, 16, 32, 32, 32, 64, 64, 64, 64, 64, 64, 64, 64, 64, 128, 128, 128, 128, 128, 128, 128};
    std::vector<unsigned> structure(divisions, divisions + 14 + 8);*/

    this->game->tower = new Tower(192, structure);

    // Create a tower builder and generate the tower with it
    this->towerBuilder = new TowerBuilder(this->game->tower);
	this->towerBuilder->isPaused=true;
	this->towerBuilder->Init();
	this->network->tb = this->towerBuilder;

    new TowerGraphics(this->game->tower, this->game->mSceneMgr);
    new TowerPhysics(this->game->tower, this->game->dynamicsWorld);

	Goal *goal = new Goal(Ogre::Vector3(0, this->game->tower->levels * this->game->tower->block_height, 0), player, this->game->mSceneMgr, this->game->dynamicsWorld);
	this->game->objects.insert(goal);
	this->goal = goal;
    
	
	Ogre::Entity *bowl = this->game->mSceneMgr->createEntity("Bowlchip.mesh");
    Ogre::SceneNode *sceneNode = this->game->mSceneMgr->getRootSceneNode()->createChildSceneNode();
    sceneNode->attachObject(bowl);
    sceneNode->setScale(30*Ogre::Vector3::UNIT_SCALE);

    btBulletWorldImporter* fileLoader = new btBulletWorldImporter(this->game->dynamicsWorld);
	fileLoader->loadFile("BowlBul.bullet");
    //btBulletWorldImporter* fileLoader = new btBulletWorldImporter(this->game->dynamicsWorld);
	//fileLoader->loadFile("BowlBul.bullet");
	this->network->startNetwork(false);

    // wait to receive game state

    // then wait to be given a player
}

void GameTestThing::startServer()
{
	isLocal = false;
	isServer = true;
	//printf("size of nettower %d\n",sizeof(NetTower));
	unsigned divisions[] = {8, 16, 16, 32, 32, 32, 64, 64, 64, 64, 64, 64, 64, 64, 64, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 256, 256, 256, 256, 256, 256, 256};
    std::vector<unsigned> structure(divisions, divisions + 14 + 8 + 11);

	
	/*unsigned divisions[] = {8, 16, 16, 32, 32, 32, 64, 64, 64, 64, 64, 64, 64, 64, 64, 128, 128, 128, 128, 128, 128, 128};
    std::vector<unsigned> structure(divisions, divisions + 14 + 8);*/



    this->game->tower = new Tower(192, structure);

    // Create a tower builder and generate the tower with it
    this->towerBuilder = new TowerBuilder(this->game->tower);
	this->towerBuilder->isPaused=true;
	this->towerBuilder->Init();
	this->towerBuilder->network = this->network;
	this->network->tb = this->towerBuilder;
	this->towerBuilder->generate();
	
    new TowerGraphics(this->game->tower, this->game->mSceneMgr);
    new TowerPhysics(this->game->tower, this->game->dynamicsWorld);
	//Ogre::Entity *bowl = this->game->mSceneMgr->createEntity("Bowlchip.mesh");
    //Ogre::SceneNode *sceneNode = this->game->mSceneMgr->getRootSceneNode()->createChildSceneNode();
    //sceneNode->attachObject(bowl);
    //sceneNode->setScale(30*Ogre::Vector3::UNIT_SCALE);

    //btBulletWorldImporter* fileLoader = new btBulletWorldImporter(this->game->dynamicsWorld);
	//fileLoader->loadFile("BowlBul.bullet");

    Player *player = new Player(Ogre::Vector3(0, this->game->tower->levels * this->game->tower->block_height + 10, 10));
    
    this->addPlayer(player, 0);
    this->setLocalPlayer(player);

	Goal *goal = new Goal(Ogre::Vector3(0, this->game->tower->levels * this->game->tower->block_height, 0), player, this->game->mSceneMgr, this->game->dynamicsWorld);
	this->game->objects.insert(goal);
	this->goal = goal;
    
	
	Ogre::Entity *bowl = this->game->mSceneMgr->createEntity("Bowlchip.mesh");
    Ogre::SceneNode *sceneNode = this->game->mSceneMgr->getRootSceneNode()->createChildSceneNode();
    sceneNode->attachObject(bowl);
    sceneNode->setScale(30*Ogre::Vector3::UNIT_SCALE);
    btBulletWorldImporter* fileLoader = new btBulletWorldImporter(this->game->dynamicsWorld);
	fileLoader->loadFile("BowlBul.bullet");
	this->network->g = this->goal;
	this->network->startNetwork(true);
    
    //// listen for new players, adding them when they come
    //this->network->signals.playerCreated.connect(boost::bind(&GameTestThing::addPlayer, this, _1));

    //// create local player
    //Player *player = new Player(Ogre::Vector3(0, 0, 0));
    //this->addPlayer(player);
    //this->setLocalPlayer(player);

    //// register local player
    //this->network->registerObject(player);

    //Player *test1 = new Player(Ogre::Vector3(100, 0, 0));
    //this->addPlayer(test1);

    //Player *test2 = new Player(Ogre::Vector3(100, 0, 100));
    //this->addPlayer(test2);

    //Player *test3 = new Player(Ogre::Vector3(100, 0, -100));
    //this->addPlayer(test3);

    //test1->signals.removed(test1);
    //test2->signals.removed(test2);
    //test3->signals.removed(test3);
}

void GameTestThing::netSendChat(std::string message)
{
	this->network->sendChat(message);
}

void GameTestThing::netSendExplosion(Ogre::Vector3 position)
{
	this->network->sendExplosion(position);
}

void GameTestThing::netSendRocket(Ogre::Vector3 position, Ogre::Quaternion orientation)
{
	this->network->sendRocket(position, orientation);
}

void GameTestThing::update()
{
    // Remove the things in the remove queue from the set of objects to be updated
    // (if we did this during normal updates then the iterator would break)
    for(std::set<Updatable *>::iterator i = this->removeQueue.begin(); i != this->removeQueue.end(); ++i)
    {
        Updatable *object = *i;

        this->game->objects.erase(object);

        delete object;
    }

    this->removeQueue.clear();

	if(this->towerBuilder != NULL)
	{
		this->towerBuilder->update();
	}

	if(this->network != NULL && this->localPlayer != NULL) {
	    this->localPlayer->setScores(this->network->teamScores);
	    //printf("---------------------%d %d %d %d\n", this->network->teamScores[0], this->network->teamScores[1], this->network->teamScores[2], this->network->teamScores[3]);
	}
}

void GameTestThing::turretFired(Turret *turret, Rocket *rocket)
{

    this->addRocket(rocket);
}

void GameTestThing::playerFired(Player *player, Rocket *rocket)
{
    if (isLocal) 
		{
			std::cout<<"Local Rocket" <<std::endl;
			this->addRocket(rocket);
	}
	this->network->sendRocket(rocket->position,rocket->orientation);
}

void GameTestThing::rocketExploded(Rocket *rocket, Explosion *explosion)
{
    this->removeQueue.insert(rocket);
    // TODO: refactor

	this->addExplosion(explosion);

    if (isServer) this->network->sendExplosion(explosion->position);
    if (isLocal) 
	{
		std::cout<<"Local Explosion" <<std::endl;
		//this->addExplosion(explosion);
	}
}

void GameTestThing::explosionFinished(Explosion *explosion)
{
    this->removeQueue.insert(explosion);
}

void GameTestThing::platformCreated(Player *player, Platform *platform)
{
	printf("in GameTestThing::platformCreated\n");
    this->game->objects.insert(platform);

    PlatformGraphics *platfromGraphics = new PlatformGraphics(platform, this->game->mSceneMgr);
	PlatformPhysics *platformPhysics = new PlatformPhysics(platform, this->game->dynamicsWorld);
	this->sounds->createPlatformSound(player);
    platform->signals.expired.connect(boost::bind(&GameTestThing::platformExpired, this, _1));
    platform->signals.destroyed.connect(boost::bind(&GameTestThing::platformExpired, this, _1));
}

void GameTestThing::platformExpired(Platform *platform)
{
    this->removeQueue.insert(platform);
	this->sounds->createPlatformStop();
}

void GameTestThing::chatReceived(std::string message)
{
    std::cout << "CHAT: " << message << std::endl;
}

void GameTestThing::networkExplosion(double x, double y, double z, bool isMassive)
{
    Explosion *explosion = new Explosion(Ogre::Vector3(x, y, z), isMassive);

    this->addExplosion(explosion);
}

void GameTestThing::networkRocket(Ogre::Vector3 position, Ogre::Quaternion orientation)
{
	Rocket* rocket = new Rocket(position,orientation);
	this->addRocket(rocket);
}

void GameTestThing::playerUsed(Player *player)
{
	Ogre::Vector3 min;
	double minDis = 1000;
	double check = 0;
	Turret *turret;
	    for(std::set<Turret *>::iterator i = this->turrets.begin(); i != this->turrets.end(); ++i)
    {
        turret = *i;
		check = (turret->position - player->position).length();
		if(minDis > check) 
		{
		    min = turret->position;
		    minDis = check;
	    }
        std::cout << turret->position << std::endl;
    }
		for(std::set<Turret *>::iterator j = this->turrets.begin(); j != this->turrets.end(); ++j)
    {
        turret = *j;
		if(min == turret->position) 
		{
            j = this->turrets.end();
	    }
    }
	/*printf("%f\n", minDis);
	printf("%f,%f,%f\n",turret->position.x, turret->position.y, turret->position.z);
	printf("%f,%f,%f\n",min.x, min.y, min.z);*/
	if(minDis < 180)
	{
	    if(player->position == min)
	    {
			turret->setOccupied(false);
			//turret->setTarget(NULL);
			player->setTurret(NULL);
	        player->exitedTurret();

	    }
	    else
	    {
	    	player->enteredTurret();
			player->setTurret(turret);
			turret->setOccupied(true);
			this->sounds->enterTurretSound(turret);
	        player->position = turret->position;

			//turret->setTarget(player);
	    }
	}
    // look at all turrets here and see if the player is getting inside one
    std::cout << "USING!" << std::endl;
}

void GameTestThing::setLocalPlayer(Player *player)
{
    this->localPlayer = player;
	this->localPlayer->prop = new PlayerProperties(this->localPlayer->prop->getTeam(), true);
	
	localPlayerGraphics->~PlayerGraphics();
    new PlayerGraphics(player, this->game->mSceneMgr);
    // link the local camera to the player
    new PlayerCamera(player, this->game->mCamera);

    // Link local sound to the player
    this->sounds->setListener(player);

    // Link local input to the player
    player->addInput(this->game->playerInput);

    // Add player physics and link local input to it
    localPlayerPhysics = new PlayerPhysics(player, this->game->dynamicsWorld);
	localPlayerPhysics->addInput(this->game->playerInput);
}

void GameTestThing::addPlayer(Player *player, int team)
{
	player->prop = new PlayerProperties(team, false);

    // Add player graphics
    localPlayerGraphics = new PlayerGraphics(player, this->game->mSceneMgr);

    // Add player to updatable objects
    this->game->objects.insert(player);

    // Listen for when the players fire or create platforms
    player->signals.fired.connect(boost::bind(&GameTestThing::playerFired, this, _1, _2));
    player->signals.platform.connect(boost::bind(&GameTestThing::platformCreated, this, _1, _2));
    player->signals.used.connect(boost::bind(&GameTestThing::playerUsed, this, _1));
    player->signals.removed.connect(boost::bind(&GameTestThing::removePlayer, this, _1));
}

void GameTestThing::removePlayer(Player *player)
{
    // Queue player for removal
	std::cout << "In GameTestThing, Removing Player Object at (" << player->position.x <<", " << player->position.y<<", " << player->position.z<< ")" << std::endl;
    this->removeQueue.insert(player);
}

void GameTestThing::addTurret(Turret *turret)
{
    // Create turret graphics
    new TurretGraphics(turret, this->game->mSceneMgr);
    
    this->turrets.insert(turret);

    // update the turrets every frame
    this->game->objects.insert(turret);

    // Listen for when the turret fires
    turret->signals.fired.connect(boost::bind(&GameTestThing::turretFired, this, _1, _2));
}

void GameTestThing::addRocket(Rocket *rocket)
{
    this->game->objects.insert(rocket);

    new RocketGraphics(rocket, this->game->mSceneMgr);
    new RocketPhysics(rocket, this->game->dynamicsWorld);
    new RocketSound(rocket, this->sounds->engine);

    rocket->signals.exploded.connect(boost::bind(&GameTestThing::rocketExploded, this, _1, _2));
}

void GameTestThing::addExplosion(Explosion *explosion)
{
	this->game->objects.insert(explosion);
	explosion->signals.finished.connect(boost::bind(&GameTestThing::explosionFinished, this, _1));
	
    new ExplosionGraphics(explosion, this->game->mSceneMgr, int(this->count));
	new ExplosionPhysics(explosion, this->game->dynamicsWorld);
	this->count = this->count++;
    
}