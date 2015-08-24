#ifndef __MADSHOOTER_SHIELDPOWERUP_HPP__
#define __MADSHOOTER_SHIELDPOWERUP_HPP__

#include "PowerUp.hpp"
#include "ShieldLevel.hpp"

class Ship;

class ShieldPowerUp : public PowerUp
{
private:
    ShieldLevel _level;
public:
    ShieldPowerUp(ShieldLevel l);
    virtual ~ShieldPowerUp();
    virtual void apply(Ship* s) override;
    virtual bool init() override;
    virtual void update(float dt) override;
    static ShieldPowerUp* create(ShieldLevel l);
friend class Ship;
};

#endif