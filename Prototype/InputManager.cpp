#include "InputManager.h"

InputManager::InputManager(Ogre::RenderWindow *window)
    : manager(NULL)
    , keyboard(NULL)
    , mouse(NULL)
{
    size_t handle = 0;
    window->getCustomAttribute("WINDOW", &handle);
    
    // OIS takes input with a funny data structure
    OIS::ParamList parameters;

    std::ostringstream handleString;
    handleString << handle;

    parameters.insert(std::make_pair(std::string("WINDOW"), handleString.str()));

    this->manager = OIS::InputManager::createInputSystem(parameters);

    //
    this->keyboard = static_cast<OIS::Keyboard *>(this->manager->createInputObject(OIS::OISKeyboard, true));
    this->mouse = static_cast<OIS::Mouse *>(this->manager->createInputObject(OIS::OISMouse, true));

    // Listen for window events
    Ogre::WindowEventUtilities::addWindowEventListener(window, this);
    
    // Set initial mouse size
    this->windowResized(window);
}

InputManager::~InputManager()
{
}

void InputManager::capture()
{
    this->mouse->capture();
    this->keyboard->capture();
}

void InputManager::windowResized(Ogre::RenderWindow *window)
{
    // Update the mouse
    unsigned width, height, depth;
    int left, top;
    
    window->getMetrics(width, height, depth, left, top);
 
    const OIS::MouseState &mouseState = this->mouse->getMouseState();
    
    mouseState.width = width;
    mouseState.height = height;
}

void InputManager::windowClosed(Ogre::RenderWindow *window)
{
    if (this->manager)
    {
        this->manager->destroyInputObject(this->mouse);
        this->manager->destroyInputObject(this->keyboard);

        OIS::InputManager::destroyInputSystem(this->manager);
        this->manager = NULL;
    }
}
