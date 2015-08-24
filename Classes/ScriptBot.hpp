#ifndef __MADSHOOTER_SCRIPTBOT_HPP__
#define __MADSHOOTER_SCRIPTBOT_HPP__

#include "Ship.hpp"

class ScriptBot : public Ship {
private:
    lua_State* L;
    std::string scriptFilename;
    bool didTheJob;
    bool _scriptLoaded;
public:
    virtual ~ScriptBot();
    virtual bool init() override;
    // MUST be called right after creating the object to set the script file that will serve behavior
    // otherwise the bot will do nothing.
    bool loadScript(std::string filename);
    virtual void update(float dt) override;
    CREATE_FUNC(ScriptBot);
    // implemented methods
    void onShipEnteredLineOfSight(ShipInfo info) override;
    void onShipStateChanged(ShipInfo info) override;
    void onShipOutOfLineOfSight(ShipInfo info) override;
    void onShipDied(int shipID) override; // Called when a ship in my line of sight dies
    void onDeath() override; // Called when this ship dies
    void onHitByOtherShip(float power, float damageInflicted, float missileAngle) override;
    void onPowerUpSpotted() override;
};

#endif