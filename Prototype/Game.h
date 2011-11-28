#ifndef __Game_h_
#define __Game_h_

#include <vector>

#include "BaseApplication.h"

#include "Object.h"
#include "Player.h"

class Game : public BaseApplication
{
public:
    Game(void);
    virtual ~Game(void);

    virtual void run(void);

protected:
    virtual void createScene(void);

    virtual bool keyPressed(const OIS::KeyEvent &arg);
    virtual bool keyReleased(const OIS::KeyEvent &arg);

private:
    std::vector<Object *> objects;
    Player *player;
};

#endif // #ifndef __Game_h_
