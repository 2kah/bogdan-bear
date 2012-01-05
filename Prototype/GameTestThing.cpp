#include "GameTestThing.h"

#include <iostream>
#include <string>

#include <boost/signal.hpp>
#include <boost/bind.hpp>

#include <OGRE/OgreVector3.h>

#include "Game.h"

#include "Player.h"
#include "PlayerGraphics.h"

#include "Platform.h"
#include "PlatformGraphics.h"

#include "Rocket.h"
#include "RocketGraphics.h"

#include "Explosion.h"
#include "ExplosionGraphics.h"

GameTestThing::GameTestThing(Game *game)
{
    this->game = game;
    
    this->player = new Player(Ogre::Vector3(0, 0, 1000));
    this->game->player = this->player;

    this->player->addToScene(this->game->mSceneMgr);
    this->player->signals.updated.connect(boost::bind(&Player::doGraphics, this->player, _1));

    new PlayerGraphics(this->player, this->game->mSceneMgr);

    Player *enemy = new Player(Ogre::Vector3(1000, 0, 1000));
    new PlayerGraphics(enemy, this->game->mSceneMgr);

    this->game->objects.insert(this->player);
    this->game->objects.insert(enemy);

    this->player->signals.fired.connect(boost::bind(&GameTestThing::playerFired, this, _1, _2));
    this->player->signals.platform.connect(boost::bind(&GameTestThing::platformCreated, this, _1, _2));

    enemy->signals.fired.connect(boost::bind(&GameTestThing::playerFired, this, _1, _2));
    enemy->signals.platform.connect(boost::bind(&GameTestThing::platformCreated, this, _1, _2));

    this->player->cameraNode->attachObject(this->game->mCamera);
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
    
    ExplosionGraphics *explosionGraphics = new ExplosionGraphics(explosion, this->game->mSceneMgr);

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
