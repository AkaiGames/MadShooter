#ifndef __MADSHOOTER_DAMAGEPOWERUP_HPP__
#define __MADSHOOTER_DAMAGEPOWERUP_HPP__

#include "PowerUp.hpp"
#include "DamageLevel.hpp"

class Ship;

class DamagePowerUp : public PowerUp
{
private:
    DamagePowerUp(DamageLevel l);
    DamageLevel _damageLevel;
public:
    virtual ~DamagePowerUp();
    virtual void apply(Ship* s) override;
    virtual bool init() override;
    virtual void update(float dt) override;
    static DamagePowerUp* create(DamageLevel l);
friend class Ship;
};

#endif