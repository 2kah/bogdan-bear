#ifndef __PlayerProperties_h_
#define __PlayerProperties_h_

#include <string>

using namespace std;

class PlayerProperties
{
public:
    PlayerProperties(int team);
    virtual ~PlayerProperties();
	virtual int getTeam();
	virtual string getCostumeName();
protected:
    int team;
};

#endif // #ifndef __PlayerProperties_h_
