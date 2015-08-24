#ifndef __MADSHOOTER_PLAYER_TWO_HPP__
#define __MADSHOOTER_PLAYER_TWO_HPP__

#include "Player.hpp"

class PlayerTwo : public Player
{
protected:
    virtual void setEventHandlers() override;
public:
    virtual ~PlayerTwo();
    virtual bool init() override;
    virtual void update(float dt) override;
    CREATE_FUNC(PlayerTwo);
};

#endif
