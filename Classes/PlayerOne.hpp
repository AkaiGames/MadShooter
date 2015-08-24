#ifndef __MADSHOOTER_PLAYER_ONE_HPP__
#define __MADSHOOTER_PLAYER_ONE_HPP__

#include "Player.hpp"

class PlayerOne : public Player
{
protected:
    virtual void setEventHandlers() override;
public:
    virtual ~PlayerOne();
    virtual bool init() override;
    virtual void update(float dt) override;
    CREATE_FUNC(PlayerOne);
};

#endif
