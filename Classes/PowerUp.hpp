#ifndef __MADSHOOTER_POWERUP_HPP__
#define __MADSHOOTER_POWERUP_HPP__

#include "Entity.hpp"
#include "Ship.hpp"

class Ship;

class PowerUp : public Entity
{
protected:
    ShipInfo _stats;
public:
    virtual ~PowerUp();
    virtual void apply(Ship* s) = 0;
    virtual bool init() override;
    virtual void update(float dt) override;
};

#endif