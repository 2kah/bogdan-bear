#ifndef __Game_h_
#define __Game_h_

#include <vector>

#include "BaseApplication.h"

#include "Object.h"

class Game : public BaseApplication
{
public:
    Game(void);
    virtual ~Game(void);

    virtual void run(void);

protected:
    virtual void createScene(void);

private:
    std::vector<Object *> objects;
};

#endif // #ifndef __Game_h_
