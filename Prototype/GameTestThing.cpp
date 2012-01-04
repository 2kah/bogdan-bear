#include "GameTestThing.h"

#include <iostream>
#include <string>

#include <boost/signal.hpp>
#include <boost/bind.hpp>

#include <OGRE/OgreVector3.h>

#include "Game.h"
#include "Player.h"
#include "Rocket.h"
#include "RocketGraphics.h"
#include "Explosion.h"

GameTestThing::GameTestThing(Game *game)
{
    this->game = game;
    
    this->timer = 0;

    this->game->player->signals.fired.connect(boost::bind(&GameTestThing::playerFired, this, _1, _2));
}

GameTestThing::~GameTestThing()
{
}

void GameTestThing::update()
{
    if (this->timer < 300)
    {
        ++this->timer;
    }
    else
    {
        this->timer = 0;
    }

    for(std::set<Updatable *>::iterator i = this->removeQueue.begin(); i != this->removeQueue.end(); ++i)
    {
        Updatable *object = *i;

        this->game->objects.erase(object);
    }

    this->removeQueue.clear();
}

void GameTestThing::playerFired(Player *player, Rocket *rocket)
{
    std::cout << "Player fired, adding rocket..." << std::endl;

    this->game->objects.insert(rocket);

    RocketGraphics *rocketGraphics = new RocketGraphics(rocket, this->game->mSceneMgr);

    rocket->signals.exploded.connect(boost::bind(&GameTestThing::rocketExploded, this, _1, _2));
}

void GameTestThing::rocketExploded(Rocket *rocket, Explosion *explosion)
{
    std::cout << "Rocket exploded, adding explosion..." << std::endl;

    this->game->objects.insert(explosion);

    explosion->signals.finished.connect(boost::bind(&GameTestThing::explosionFinished, this, _1));

    static int i = 0;
    std::stringstream name("explosion");
    name << i;
    ++i;

    explosion->addToScene(this->game->mSceneMgr, name.str());

    this->removeQueue.insert(rocket);
}

void GameTestThing::explosionFinished(Explosion *explosion)
{
    explosion->removeFromScene(this->game->mSceneMgr);
}
