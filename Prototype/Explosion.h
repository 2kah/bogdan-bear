#ifndef __Explosion_h_
#define __Explosion_h_

#include "SceneObject.h"

#include "Game.h"

class Explosion: public SceneObject
{
public:
    Explosion(Game *game, Ogre::Vector3 position);
    virtual ~Explosion(void);

    virtual void addToScene(Ogre::SceneManager *sceneMgr, std::string name);
    virtual void update(void);

protected:
    int size;
    Game *game;
};

#endif // #ifndef __Explosion_h_
