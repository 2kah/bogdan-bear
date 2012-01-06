#ifndef __PlayerPhysics_h_
#define __PlayerPhysics_h_

class Player;
class btDiscreteDynamicsWorld;

class PlayerPhysics
{
public:
    PlayerPhysics(Player *player, btDiscreteDynamicsWorld *dynamicsWorld);
    virtual ~PlayerPhysics();
protected:
    virtual void playerUpdated(Player *player);

    Player *player;
};

#endif // #ifndef __PlayerPhysics_h_
