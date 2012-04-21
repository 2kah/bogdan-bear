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
	Ogre::SceneNode *gunNode;
	Ogre::SceneNode *camNode;
    Ogre::Entity *entity;
	Ogre::AnimationState *animationState;

	Ogre::Entity *team1;
	Ogre::Entity *team2;
	Ogre::Entity *team3;
	Ogre::Entity *team4;
	Ogre::Entity *teamWin;
	Ogre::Entity *ammo1;
	Ogre::Entity *ammo2;
	Ogre::Entity *ammo3;
	Ogre::Entity *ammo4;

	double check;
};

#endif // #ifndef __PlayerGraphics_h_
