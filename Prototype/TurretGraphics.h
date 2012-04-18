#ifndef __TurretGraphics_h_
#define __TurretGraphics_h_

#include <OGRE/OgreSceneManager.h>
#include <OGRE/OgreSceneNode.h>
#include <OGRE/OgreEntity.h>

class Turret;
class Rocket;

class TurretGraphics
{
public:
    TurretGraphics(Turret *player, Ogre::SceneManager *sceneManager);
    virtual ~TurretGraphics();
protected:
    virtual void turretUpdated(Turret *turret);
    virtual void turretFired(Turret *turret, Rocket *rocket);

    Turret *turret;

    Ogre::SceneManager *sceneManager;
    Ogre::SceneNode *sceneNodeBase;
	Ogre::SceneNode *sceneNodeGuard;
	Ogre::SceneNode *sceneNodeGuns;
    Ogre::Entity *entity;
};

#endif // #ifndef __TurretGraphics_h_
