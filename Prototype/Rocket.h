#ifndef __Rocket_h_
#define __Rocker_h_

#include <OgreVector3.h>

class Rocket
{
public:
    Rocket(Ogre::Vector3 position);
    virtual ~Rocket(void);
protected:
    Ogre::Vector3 position;
    Ogre::Vector3 orientation;
};

#endif // #ifndef __Rocket_h_
