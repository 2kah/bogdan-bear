#ifndef __Updatable_h_
#define __Updatable_h_

class Updatable
{
public:
    virtual ~Updatable() {};

    virtual void update() = 0;
};

#endif // #ifndef __Updatable_h_
