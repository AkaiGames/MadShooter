#ifndef __MADSHOOTER_HEALTHPOWERUP_HPP__
#define __MADSHOOTER_HEALTHPOWERUP_HPP__

#include "PowerUp.hpp"

class Ship;

class HealthPowerUp : public PowerUp
{
public:
    virtual ~HealthPowerUp();
    virtual void apply(Ship* s) override;
    virtual bool init() override;
    virtual void update(float dt) override;
    CREATE_FUNC(HealthPowerUp);
friend class Ship;
};

#endif