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

#include "Platform.h"
#include "PlatformGraphics.h"
#include "PlatformPhysics.h"

#include "Rocket.h"
#include "RocketGraphics.h"
#include "RocketPhysics.h"
#include "RocketSound.h"

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
    this->network->signals.chat.connect(boost::bind(&GameTestThing::chatReceived, this, _1));
    this->network->signals.explosion.connect(boost::bind(&GameTestThing::networkExplosion, this, _1, _2, _3));
	this->network->signals.recvRocket.connect(boost::bind(&GameTestThing::networkRocket, this, _1, _2));
    this->game->objects.insert(this->network);

	this->network->signals.addPlayer.connect(boost::bind(&GameTestThing::addPlayer, this, _1));
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
    Ogre::Entity *bowl = this->game->mSceneMgr->createEntity("Bowl.mesh");
    Ogre::SceneNode *sceneNode = this->game->mSceneMgr->getRootSceneNode()->createChildSceneNode();
    sceneNode->attachObject(bowl);
    //sceneNode->setScale(30*Ogre::Vector3::UNIT_SCALE);

    btBulletWorldImporter* fileLoader = new btBulletWorldImporter(this->game->dynamicsWorld);
	fileLoader->loadFile("bowl.bullet");

    // Create and add a falling object
    /*FallingObject *object = new FallingObject(Ogre::Vector3(40.5, 64, 40.15));
    object->addToScene(this->game->mSceneMgr);
    object->addToPhysics(this->game->dynamicsWorld);
    this->game->objects.insert(object);*/

    this->game->mSceneMgr->setAmbientLight(Ogre::ColourValue(0.5, 0.5, 0.5));

    Ogre::Light *moon = this->game->mSceneMgr->createLight();
    moon->setType(Ogre::Light::LT_DIRECTIONAL);
    moon->setDirection(Ogre::Vector3(0.5, -1, 0.5));
    moon->setDiffuseColour(Ogre::ColourValue::White);
    moon->setSpecularColour(Ogre::ColourValue::White);

    std::cout << "PRESS F11 FOR LOCAL TEST GAME" << std::endl;
    std::cout << "PRESS F9 FOR LOCAL SERVER GAME" << std::endl;
    std::cout << "PRESS F8 FOR LOCAL CLIENT GAME" << std::endl;

}

GameTestThing::~GameTestThing()
{
}

void GameTestThing::startLocal()
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

    // Add tower graphics and physics
    new TowerGraphics(this->game->tower, this->game->mSceneMgr);
    new TowerPhysics(this->game->tower, this->game->dynamicsWorld);

    // Add a player
    Player *player = new Player(Ogre::Vector3(0, this->game->tower->levels * this->game->tower->block_height + 10, 10));
    Player *enemy = new Player(Ogre::Vector3(100, 0, 100));

    this->addPlayer(player);
    this->setLocalPlayer(player);

    this->addPlayer(enemy);

	//Add the goal
	Goal *goal = new Goal(Ogre::Vector3(0, this->game->tower->levels * this->game->tower->block_height, 0), player);
	this->game->objects.insert(goal);
	this->goal = goal;

    // Add four turrets
    Turret *turret1 = new Turret(Ogre::Vector3(0, 150, 400), Ogre::Quaternion());
	Turret *turret2 = new Turret(Ogre::Vector3(0, 150, -400), Ogre::Quaternion());
	Turret *turret3 = new Turret(Ogre::Vector3(400, 150, 0), Ogre::Quaternion());
	Turret *turret4 = new Turret(Ogre::Vector3(-400, 150, 0), Ogre::Quaternion());

    this->addTurret(turret1);
    this->addTurret(turret2);
    this->addTurret(turret3);
    this->addTurret(turret4);
    

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
	this->network->startNetwork(false);

    // wait to receive game state

    // then wait to be given a player
}

void GameTestThing::startServer()
{
	isLocal = false;
	isServer = true;

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
    }

    this->removeQueue.clear();

	if(this->towerBuilder != NULL)
	{
		this->towerBuilder->update();
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
    if (isServer) this->network->sendExplosion(explosion->position);

    if (isLocal) 
	{
		std::cout<<"Local Explosion" <<std::endl;
		this->addExplosion(explosion);
	}
}

void GameTestThing::explosionFinished(Explosion *explosion)
{
    this->removeQueue.insert(explosion);
}

void GameTestThing::platformCreated(Player *player, Platform *platform)
{
    this->game->objects.insert(platform);

    PlatformGraphics *platfromGraphics = new PlatformGraphics(platform, this->game->mSceneMgr);
	PlatformPhysics *platformPhysics = new PlatformPhysics(platform, this->game->dynamicsWorld);

    platform->signals.expired.connect(boost::bind(&GameTestThing::platformExpired, this, _1));
    platform->signals.destroyed.connect(boost::bind(&GameTestThing::platformExpired, this, _1));
}

void GameTestThing::platformExpired(Platform *platform)
{
    this->removeQueue.insert(platform);
}

void GameTestThing::chatReceived(std::string message)
{
    std::cout << "CHAT: " << message << std::endl;
}

void GameTestThing::networkExplosion(double x, double y, double z)
{
    Explosion *explosion = new Explosion(Ogre::Vector3(x, y, z));

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
	printf("%f\n", minDis);
	printf("%f,%f,%f\n",turret->position.x, turret->position.y, turret->position.z);
	printf("%f,%f,%f\n",min.x, min.y, min.z);
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

    // link the local camera to the player
    new PlayerCamera(player, this->game->mCamera);

    // Link local sound to the player
    this->sounds->setListener(player);

    // Link local input to the player
    player->addInput(this->game->playerInput);

    // Add player physics and link local input to it
    PlayerPhysics* playerPhysics = new PlayerPhysics(player, this->game->dynamicsWorld);
	playerPhysics->addInput(this->game->playerInput);
}

void GameTestThing::addPlayer(Player *player)
{
	player->prop = new PlayerProperties(0);

    // Add player graphics
    new PlayerGraphics(player, this->game->mSceneMgr);
	

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
    
    new ExplosionGraphics(explosion, this->game->mSceneMgr);
    new ExplosionPhysics(explosion, this->game->dynamicsWorld);
}
