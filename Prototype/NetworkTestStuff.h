#ifndef __NetworkTestStuff_h_
#define __NetworkTestStuff_h_

#include <string>

#include <boost/signals.hpp>

#include "Updatable.h"

namespace {
struct NetworkSignals {
    boost::signal<void (std::string message)> chat;
};
}

class NetworkTestStuff : public Updatable
{
public:
    NetworkTestStuff();
    virtual ~NetworkTestStuff();

    NetworkSignals signals;

    virtual void update();
};

#endif // #ifndef __NetworkTestStuff_h_
