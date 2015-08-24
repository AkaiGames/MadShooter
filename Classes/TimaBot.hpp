#ifndef __MADSHOOTER_TIMABOT_HPP__
#define __MADSHOOTER_TIMABOT_HPP__

#include "Ship.hpp"

class TimaBot : public Ship {
private:
    bool _seeking;
    ShipInfo _targetInfo;
public:
    virtual ~TimaBot();
    virtual bool init() override;
    virtual void update(float dt) override;
    CREATE_FUNC(TimaBot);
    // implemented methods
    virtual void onShipEnteredLineOfSight(ShipInfo info) override;
    virtual void onShipStateChanged(ShipInfo info) override;
    virtual void onShipOutOfLineOfSight(ShipInfo info) override;
    virtual void onShipDied(int shipID) override; // Called when a ship in my line of sight dies
    virtual void onDeath() override; // Called when this ship dies
    virtual void onHitByOtherShip(float power, float damageInflicted, float missileAngle) override;
    virtual void onPowerUpSpotted() override;
};

#endif