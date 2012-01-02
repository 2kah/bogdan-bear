#ifndef __Rocket_h_
#define __Rocker_h_

#include "Object.h"

#include <OgreVector3.h>

class Rocket: public Object
{
public:
    Rocket(Ogre::Vector3 position);
    virtual ~Rocket(void);
};

#endif // #ifndef __Rocket_h_
