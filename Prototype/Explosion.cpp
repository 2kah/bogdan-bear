#include <iostream>

#include <OgreSceneManager.h>
#include <OgreEntity.h>

#include "Explosion.h"

const double Explosion::SIZE = 10.0;

Explosion::Explosion(Ogre::Vector3 position): size(0)
{
    this->position = position;
}

Explosion::~Explosion(void)
{
}

void Explosion::update(void)
{
    if (size < 100)
    {
        ++size;
    
        this->signals.updated(this);
    }
    else
    {
        this->signals.finished(this);
    }
}
