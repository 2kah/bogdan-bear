#ifndef __PlayerInput_h_
#define __PlayerInput_h_

#include <boost/signal.hpp>

//namespace {
enum DIRECTION
{
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};
//}

namespace {
struct PlayerInputSignals {
    boost::signal<void (DIRECTION direction, bool state)> move;
    boost::signal<void (int x, int y)> look;

    boost::signal<void (bool state)> fire;
    boost::signal<void (bool state)> create;
    boost::signal<void (bool state)> jump;
    boost::signal<void (bool state)> use;
};
}

class PlayerInput
{
public:
    PlayerInputSignals signals;
};

#endif // #ifndef __PlayerInput_h_