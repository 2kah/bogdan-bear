#include "GameTestThing.h"

#include <iostream>
#include <string>

#include <boost/signal.hpp>
#include <boost/bind.hpp>

#include <OGRE/OgreVector3.h>

#include "Game.h"

#include "Turret.h"
#include "TurretGraphics.h"

#include "Player.h"
#include "PlayerPhysics.h"
#include "PlayerGraphics.h"
#include "PlayerCamera.h"

#include "Platform.h"
#include "PlatformGraphics.h"

#include "Rocket.h"
#include "RocketGraphics.h"

#include "Explosion.h"
#include "ExplosionPhysics.h"
#include "ExplosionGraphics.h"

GameTestThing::GameTestThing(Game *game)
{
    this->game = game;
    
    this->player = new Player(Ogre::Vector3(0, 0, 1000 / 16.0));
    this->game->player = this->player;

    this->game->player->addInput(this->game->playerInput);

    PlayerPhysics* playerPhysics = new PlayerPhysics(this->player, this->game->dynamicsWorld);
	playerPhysics->addInput(this->game->playerInput);

    new PlayerGraphics(this->player, this->game->mSceneMgr);
    new PlayerCamera(this->player, this->game->mCamera);

    Player *enemy = new Player(Ogre::Vector3(1000 / 16.0, 0, 1000 / 16.0));
	//new PlayerPhysics(enemy, this->game->dynamicsWorld);
    new PlayerGraphics(enemy, this->game->mSceneMgr);

    this->game->objects.insert(this->player);
    this->game->objects.insert(enemy);

    Turret *turret = new Turret(Ogre::Vector3(0, 0, 256), Ogre::Quaternion::IDENTITY);
    new TurretGraphics(turret, this->game->mSceneMgr);
    this->game->objects.insert(turret);

    turret->signals.fired.connect(boost::bind(&GameTestThing::turretFired, this, _1, _2));

    this->player->signals.fired.connect(boost::bind(&GameTestThing::playerFired, this, _1, _2));
    this->player->signals.platform.connect(boost::bind(&GameTestThing::platformCreated, this, _1, _2));

    enemy->signals.fired.connect(boost::bind(&GameTestThing::playerFired, this, _1, _2));
    enemy->signals.platform.connect(boost::bind(&GameTestThing::platformCreated, this, _1, _2));
}

GameTestThing::~GameTestThing()
{
}

void GameTestThing::update()
{
    for(std::set<Updatable *>::iterator i = this->removeQueue.begin(); i != this->removeQueue.end(); ++i)
    {
        Updatable *object = *i;

        this->game->objects.erase(object);
    }

    this->removeQueue.clear();
}

void GameTestThing::turretFired(Turret *turret, Rocket *rocket)
{
    this->game->objects.insert(rocket);

    RocketGraphics *rocketGraphics = new RocketGraphics(rocket, this->game->mSceneMgr);

    rocket->signals.exploded.connect(boost::bind(&GameTestThing::rocketExploded, this, _1, _2));
}

void GameTestThing::playerFired(Player *player, Rocket *rocket)
{
    this->game->objects.insert(rocket);

    RocketGraphics *rocketGraphics = new RocketGraphics(rocket, this->game->mSceneMgr);

    rocket->signals.exploded.connect(boost::bind(&GameTestThing::rocketExploded, this, _1, _2));
}

void GameTestThing::rocketExploded(Rocket *rocket, Explosion *explosion)
{
    this->game->objects.insert(explosion);

    explosion->signals.finished.connect(boost::bind(&GameTestThing::explosionFinished, this, _1));
    
    new ExplosionGraphics(explosion, this->game->mSceneMgr);
    new ExplosionPhysics(explosion, this->game->dynamicsWorld);

    this->removeQueue.insert(rocket);
}

void GameTestThing::explosionFinished(Explosion *explosion)
{
    this->removeQueue.insert(explosion);
}

void GameTestThing::platformCreated(Player *player, Platform *platform)
{
    this->game->objects.insert(platform);

    PlatformGraphics *platfromGraphics = new PlatformGraphics(platform, this->game->mSceneMgr);

    platform->signals.expired.connect(boost::bind(&GameTestThing::platformExpired, this, _1));
}

void GameTestThing::platformExpired(Platform *platform)
{
    this->removeQueue.insert(platform);
}
