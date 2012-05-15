#include "Platform.h"

#include <OGRE/OgreVector3.h>
#include <OGRE/OgreQuaternion.h>

Platform::Platform(Ogre::Vector3 position, Ogre::Quaternion orientation)
{
    this->position = position;
    this->orientation = orientation;

    this->timer = 0;
}

Platform::~Platform()
{
}

void Platform::update()
{
    this->signals.updated(this);

    if (this->timer >= 500)
    {
        this->timer = 0;

        this->signals.expired(this);
    }
    else
    {
        ++this->timer;
    }
}

void Platform::destroy()
{
    this->signals.expired(this);
}
