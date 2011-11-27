#ifndef __Object_h_
#define __Object_h_

class Object
{
public:
    virtual ~Object(void) {};

    virtual void update(void) = 0;
};

#endif // #ifndef __Object_h_
