#ifndef __Player_h_
#define __Player_h_

#include "Object.h"

class Player: public Object
{
public:
    Player(Ogre::SceneManager *sceneMgr, Ogre::Vector3 position, std::string name);
    virtual ~Player(void);

    virtual void update(void);
};

#endif // #ifndef __Player_h_
