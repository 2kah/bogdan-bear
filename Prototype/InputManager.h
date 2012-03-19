#ifndef __InputManager_h_
#define __InputManager_h_

#include <OGRE/OgreRenderWindow.h>
#include <OgreWindowEventUtilities.h>

#include <OISEvents.h>
#include <OISInputManager.h>
#include <OISKeyboard.h>
#include <OISMouse.h>

class InputManager : public Ogre::WindowEventListener
{
public:
    InputManager(Ogre::RenderWindow *window);
    virtual ~InputManager();

    virtual void capture();

    OIS::InputManager *manager;
    OIS::Keyboard *keyboard;
    OIS::Mouse *mouse;
protected:
    virtual void windowResized(Ogre::RenderWindow *window);
    virtual void windowClosed(Ogre::RenderWindow *window);
};

#endif // #ifndef __InputManager_h_
