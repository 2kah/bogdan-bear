#ifndef __Game_h_
#define __Game_h_

#include <vector>
#include <set>

#include "BaseApplication.h"

#include "Object.h"
#include "Player.h"
#include "Tower.h"
#include "FallingObject.h"

class Game : public BaseApplication
{
public:
    Game(void);
    virtual ~Game(void);

    virtual void run(void);

    virtual void removeSceneObject(SceneObject *);

    virtual void carveSphere(Ogre::Vector3 position, double radius);

protected:
    virtual void createScene(void);

    virtual bool keyPressed(const OIS::KeyEvent &arg);
    virtual bool keyReleased(const OIS::KeyEvent &arg);

    virtual bool mouseMoved(const OIS::MouseEvent &arg);
    virtual bool mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
    virtual bool mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id);

private:
    Player *player;
    FallingObject *fallingObject;
    btDiscreteDynamicsWorld* dynamicsWorld;

    std::set<Object *> objects;
    Tower* tower;
    
    std::set<SceneObject *> sceneRemoveQueue;
};

#endif // #ifndef __Game_h_
