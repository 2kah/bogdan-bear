#ifndef __Updatable_h_
#define __Updatable_h_

class Updatable
{
public:
    virtual ~Updatable(void) {};

    virtual void update(void) = 0;
};

#endif // #ifndef __Updatable_h_
