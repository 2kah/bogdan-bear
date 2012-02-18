#ifndef __PlayerCamera_h_
#define __PlayerCamera_h_

#include <OGRE/OgreSceneNode.h>
#include <OGRE/OgreCamera.h>

class Player;

class PlayerCamera
{
public:
    PlayerCamera(Player *player, Ogre::Camera *camera);
    virtual ~PlayerCamera();
protected:
    virtual void playerRemoved(Player *player);
    virtual void playerUpdated(Player *player);

    Player *player;
    Ogre::Camera *camera;

    Ogre::SceneNode *sceneNode;
};

#endif // #ifndef __PlayerCamera_h_
