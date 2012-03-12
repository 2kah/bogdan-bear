#ifndef __PlayerGraphics_h_
#define __PlayerGraphics_h_

#include <boost/signals.hpp>

#include <OGRE/OgreSceneManager.h>
#include <OGRE/OgreSceneNode.h>
#include <OGRE/OgreEntity.h>

class Player;

class PlayerGraphics : public boost::signals::trackable
{
public:
    PlayerGraphics(Player *player, Ogre::SceneManager *sceneManager);
    virtual ~PlayerGraphics();
protected:
    virtual void playerRemoved(Player *player);
    virtual void playerUpdated(Player *player);

    Player *player;

    Ogre::SceneManager *sceneManager;
    Ogre::SceneNode *sceneNode;
    Ogre::Entity *entity;
};

#endif // #ifndef __PlayerGraphics_h_
