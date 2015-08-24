#ifndef __MADSHOOTER_ENERGYPOWERUP_HPP__
#define __MADSHOOTER_ENERGYPOWERUP_HPP__

#include "BeamType.hpp"
#include "PowerUp.hpp"

class Ship;

class EnergyPowerUp : public PowerUp
{
private:
    EnergyPowerUp(BeamType t);
    BeamType _beamType;
public:
    virtual ~EnergyPowerUp();
    virtual void apply(Ship* s) override;
    virtual bool init() override;
    virtual void update(float dt) override;
    static EnergyPowerUp* create(BeamType t);
friend class Ship;
};

#endif