#ifndef __Game_h_
#define __Game_h_

#include "BaseApplication.h"

class Game : public BaseApplication
{
public:
    Game(void);
    virtual ~Game(void);

    virtual void run(void);

protected:
    virtual void createScene(void);
};

#endif // #ifndef __Game_h_
