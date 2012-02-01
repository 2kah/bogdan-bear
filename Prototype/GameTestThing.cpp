#include "GameTestThing.h"

#include <iostream>
#include <string>
#include <vector>

#include <boost/signal.hpp>
#include <boost/bind.hpp>

#include <OGRE/OgreVector3.h>

#include <btBulletWorldImporter.h>

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

#include "Platform.h"
#include "PlatformGraphics.h"
#include "PlatformPhysics.h"

#include "Rocket.h"
#include "RocketGraphics.h"
#include "RocketPhysics.h"

#include "Explosion.h"
#include "ExplosionPhysics.h"
#include "ExplosionGraphics.h"

GameTestThing::GameTestThing(Game *game)
{
    this->game = game;
    
    this->network = new NetworkTestStuff();
    this->network->signals.chat.connect(boost::bind(&GameTestThing::chatReceived, this, _1));
    this->network->signals.explosion.connect(boost::bind(&GameTestThing::networkExplosion, this, _1, _2, _3));
    this->game->objects.insert(this->network);

    // Create an empty tower
    unsigned divisions[] = {8, 16, 16, 32, 32, 32, 64, 64, 64, 64, 64, 64, 64, 64, 64, 128, 128, 128, 128, 128, 128, 128};
    std::vector<unsigned> structure(divisions, divisions + 14 + 8);
    //unsigned divisions[] = {32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32};
    //std::vector<unsigned> structure(divisions, divisions + 14);

    this->game->tower = new Tower(256, structure);

    // Create a tower builder and generate the tower with it
    TowerBuilder *builder = new TowerBuilder(this->game->tower);
    builder->regenerate();

    // Add the tower builder to the set of things to update
    //this->game->objects.insert(builder);

    // Add tower graphics and physics
    new TowerGraphics(this->game->tower, this->game->mSceneMgr);
    new TowerPhysics(this->game->tower, this->game->dynamicsWorld);

    // Add the bowl
    Ogre::Entity *bowl = this->game->mSceneMgr->createEntity("Bowl.mesh");
    Ogre::SceneNode *sceneNode = this->game->mSceneMgr->getRootSceneNode()->createChildSceneNode();
    sceneNode->attachObject(bowl);
    //sceneNode->setScale(30*Ogre::Vector3::UNIT_SCALE);

	btBulletWorldImporter* fileLoader = new btBulletWorldImporter(this->game->dynamicsWorld);
	fileLoader->loadFile("bowl.bullet");

    // Add a player
    this->player = new Player(Ogre::Vector3(0, this->game->tower->levels * this->game->tower->block_height + 10, 10));
    this->game->player = this->player;

    // Link local input to the player
    this->game->player->addInput(this->game->playerInput);

    // Add player physics and link local input to it
    PlayerPhysics* playerPhysics = new PlayerPhysics(this->player, this->game->dynamicsWorld);
	playerPhysics->addInput(this->game->playerInput);

    // Add player graphics and link the local camera to the player
    new PlayerGraphics(this->player, this->game->mSceneMgr);
    new PlayerCamera(this->player, this->game->mCamera);

    // Create another player with physics and graphics
    Player *enemy = new Player(Ogre::Vector3(1000 / 16.0, 0, 1000 / 16.0));
	new PlayerPhysics(enemy, this->game->dynamicsWorld);
    new PlayerGraphics(enemy, this->game->mSceneMgr);

    // Add both players to the set of things to update
    this->game->objects.insert(this->player);
    this->game->objects.insert(enemy);

    ///*
    // Create a turret with graphics and add it to the list of things to update
    Turret *turret = new Turret(Ogre::Vector3(0, 150, 400), Ogre::Quaternion::IDENTITY);
	Turret *turret2 = new Turret(Ogre::Vector3(0, 150, -400), Ogre::Quaternion::IDENTITY);
	Turret *turret3 = new Turret(Ogre::Vector3(400, 150, 0), Ogre::Quaternion::IDENTITY);
	Turret *turret4 = new Turret(Ogre::Vector3(-400, 150, 0), Ogre::Quaternion::IDENTITY);
    new TurretGraphics(turret, this->game->mSceneMgr);
	new TurretGraphics(turret2, this->game->mSceneMgr);
	new TurretGraphics(turret3, this->game->mSceneMgr);
	new TurretGraphics(turret4, this->game->mSceneMgr);
    this->game->objects.insert(turret);
	this->game->objects.insert(turret2);
	this->game->objects.insert(turret3);
	this->game->objects.insert(turret4);
    
    // Set the turret to aim at the player always. Setting it to NULL makes it shoot randomly at the tower.
	turret->setTarget(this->game->player);
	turret2->setTarget(this->game->player);
	turret3->setTarget(this->game->player);
	turret4->setTarget(this->game->player);
	
	/*turret->setTarget(NULL);
	turret2->setTarget(NULL);
	turret3->setTarget(NULL);
	turret4->setTarget(NULL);*/

    // Listen for when the turret fires
    turret->signals.fired.connect(boost::bind(&GameTestThing::turretFired, this, _1, _2));
	turret2->signals.fired.connect(boost::bind(&GameTestThing::turretFired, this, _1, _2));
	turret3->signals.fired.connect(boost::bind(&GameTestThing::turretFired, this, _1, _2));
	turret4->signals.fired.connect(boost::bind(&GameTestThing::turretFired, this, _1, _2));
    //*/

    this->sounds = new Sounds(this->player);

    // Listen for when the players fire or create platforms
    this->player->signals.fired.connect(boost::bind(&GameTestThing::playerFired, this, _1, _2));
    this->player->signals.platform.connect(boost::bind(&GameTestThing::platformCreated, this, _1, _2));

    enemy->signals.fired.connect(boost::bind(&GameTestThing::playerFired, this, _1, _2));
    enemy->signals.platform.connect(boost::bind(&GameTestThing::platformCreated, this, _1, _2));
}

GameTestThing::~GameTestThing()
{
}

void GameTestThing::netStartClient()
{
	this->network->startNetwork(false);
}
void GameTestThing::netStartServer()
{
	this->network->startNetwork(true);
}
void GameTestThing::netSendChat(std::string message)
{
	this->network->sendChat(message);
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
}

void GameTestThing::turretFired(Turret *turret, Rocket *rocket)
{
    // factor out?
    float x1 = turret->position.x;
	float y1 = turret->position.y;
	float z1 = turret->position.z;
	irrklang::vec3df position(x1, y1, z1);
    
	this->sounds->engine->play3D("sounds/play.mp3", position);
    //
    
    this->game->objects.insert(rocket);

    new RocketGraphics(rocket, this->game->mSceneMgr);
    new RocketPhysics(rocket, this->game->dynamicsWorld);

    rocket->signals.exploded.connect(boost::bind(&GameTestThing::rocketExploded, this, _1, _2));
}

void GameTestThing::playerFired(Player *player, Rocket *rocket)
{
    this->game->objects.insert(rocket);

    new RocketGraphics(rocket, this->game->mSceneMgr);
    new RocketPhysics(rocket, this->game->dynamicsWorld);

    rocket->signals.exploded.connect(boost::bind(&GameTestThing::rocketExploded, this, _1, _2));
}

void GameTestThing::rocketExploded(Rocket *rocket, Explosion *explosion)
{
    this->game->objects.insert(explosion);

    explosion->signals.finished.connect(boost::bind(&GameTestThing::explosionFinished, this, _1));
    
    new ExplosionGraphics(explosion, this->game->mSceneMgr);
    new ExplosionPhysics(explosion, this->game->dynamicsWorld);

    this->removeQueue.insert(rocket);

    // TODO: refactor
    this->network->sendExplosion(explosion->position.x, explosion->position.y, explosion->position.z);
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

    this->game->objects.insert(explosion);

    explosion->signals.finished.connect(boost::bind(&GameTestThing::explosionFinished, this, _1));
    
    new ExplosionGraphics(explosion, this->game->mSceneMgr);
    new ExplosionPhysics(explosion, this->game->dynamicsWorld);
}
