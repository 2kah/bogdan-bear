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
    
    this->game->player->signals.fired.connect(boost::bind(&GameTestThing::playerFired, this, _1, _2));
    this->game->player->signals.platform.connect(boost::bind(&GameTestThing::platformCreated, this, _1, _2));
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
