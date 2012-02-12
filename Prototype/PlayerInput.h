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

	//TODO: remove these 2 (testing purposes only)
	boost::signal<void (bool state)> deactivate;
	boost::signal<void (bool state)> reactivate;
};
}

class PlayerInput
{
public:
    PlayerInputSignals signals;
};

#endif // #ifndef __PlayerInput_h_