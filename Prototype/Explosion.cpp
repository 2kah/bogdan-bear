#include <iostream>

#include <OgreSceneManager.h>
#include <OgreEntity.h>

#include "Explosion.h"

const double Explosion::SIZE = 10.0;

Explosion::Explosion(Ogre::Vector3 position, bool isMassive)
	: size(0)
	, isMassive(isMassive)
{
    this->position = position;
}

Explosion::Explosion(Ogre::Vector3 position)
	: size(0)
	, isMassive(false)
{
	this->position = position;
}

Explosion::~Explosion(void)
{
}

void Explosion::update(void)
{
    if (size < 100 && !isMassive)
    {
        ++size;
    
        this->signals.updated(this);
    }
	else if (size < 200 && isMassive)
	{
		++size;

		this->signals.updated(this);
	}
    else
    {
        this->signals.finished(this);
    }
}
