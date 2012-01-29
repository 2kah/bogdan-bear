#include "NetworkTestStuff.h"

NetworkTestStuff::NetworkTestStuff()
{
}

NetworkTestStuff::~NetworkTestStuff()
{
}

void NetworkTestStuff::update()
{
    this->signals.chat("TESTING!");
}
