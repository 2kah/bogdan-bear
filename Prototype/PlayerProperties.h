#ifndef __PlayerProperties_h_
#define __PlayerProperties_h_

#include <string>

using namespace std;

class PlayerProperties
{
public:
    PlayerProperties(int team, bool isLocal);
    virtual ~PlayerProperties();
	virtual int getTeam();
	virtual string getCostumeName();
	virtual string getGunPlatformsName();
	virtual string getGunAmmoName();
	virtual bool isPlayerLocal();
protected:
    int team;
	bool isLocal;
};

#endif // #ifndef __PlayerProperties_h_
