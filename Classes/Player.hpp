#ifndef __MADSHOOTER_PLAYER_HPP__
#define __MADSHOOTER_PLAYER_HPP__

#include "Ship.hpp"

class Player : public Ship
{
protected:
    int _turningDirection; // 0 - STOPPED; 1 - RIGHT; -1 - LEFT;
    // set the event handlers for the player - keyboard, mouse, touch
    virtual void setEventHandlers() = 0;
public:
    virtual ~Player();
    virtual bool init() override;
    virtual void update(float dt) override;
    virtual void onShipEnteredLineOfSight(ShipInfo info) override;
    virtual void onShipStateChanged(ShipInfo info) override;
    virtual void onShipOutOfLineOfSight(ShipInfo info) override;
    virtual void onShipDied(int shipID) override; // Called when a ship in my line of sight dies
    virtual void onDeath() override; // Called when this ship dies
    virtual void onHitByOtherShip(float power, float damageInflicted, float missileAngle) override;
    virtual void onPowerUpSpotted() override;
};

#endif
