#ifndef __Explosion_h_
#define __Explosion_h_

#include "SceneObject.h"

class Explosion: public SceneObject
{
public:
    Explosion(Ogre::Vector3 position);
    virtual ~Explosion(void);

    virtual void addToScene(Ogre::SceneManager *sceneMgr, std::string name);
    virtual void update(void);
};

#endif // #ifndef __Explosion_h_
