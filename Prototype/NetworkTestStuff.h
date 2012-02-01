#ifndef __NetworkTestStuff_h_
#define __NetworkTestStuff_h_

#include <string>

#include <boost/signals.hpp>

#include "Updatable.h"

namespace {
struct NetworkSignals {
    boost::signal<void (std::string message)> chat;
    boost::signal<void (double x, double y, double z)> explosion;
};
}

class NetworkTestStuff : public Updatable
{
public:
    NetworkTestStuff();
    virtual ~NetworkTestStuff();

    NetworkSignals signals;

	virtual void startNetwork(bool asServer);
	virtual void sendChat(std::string message);
    virtual void update();

    void startServer();
    void startClient();

    virtual void sendExplosion(double x, double y, double z);
};

#endif // #ifndef __NetworkTestStuff_h_
