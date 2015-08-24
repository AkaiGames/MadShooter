#include "TimaBot.hpp"

USING_NS_CC;
using namespace luabridge;

TimaBot::~TimaBot()
{
    log("~TimaBot(T=%d,ID=%d)", getType(), getID());
}

bool TimaBot::init()
{
    if ( Ship::init() == false )
        return false;
    
    _seeking = true;
    
    // Tells Cocos2d-x to call the Node's update function
    scheduleUpdate();
    
    return true;
}

void TimaBot::update(float dt)
{
    Ship::update(dt);
    
    if ( _seeking == true && isTurning() == false )
    {
        turnShip(180);
    }
    else if ( _seeking == false ) 
    {
        fire();
    }
    
    if ( _seeking == false && Entity::isAlive( _targetInfo.ID ) == false ) 
    {
        log("[Tima-%d] My target died!!! hahaha", getID());
        _seeking = true;
    }
}

void TimaBot::onShipStateChanged(ShipInfo info)
{
    log("1-[Tima-%d]: Ship-%d changed.", getID(), info.ID);
}

void TimaBot::onShipOutOfLineOfSight(ShipInfo info)
{
    _seeking = true;
}

void TimaBot::onShipEnteredLineOfSight(ShipInfo info)
{
    if ( _seeking == true ) 
    {
        log("[Tima-%d] Aiming at %d!", getID(), info.ID);
        _targetInfo = info;
        _seeking = false;
        stopTurning();
        turnShip(15);
        fire();
    } else {
        log("[Tima-%d] I have no time for %d, my target is: %d!", getID(), info.ID, _targetInfo.ID);
    }
}

void TimaBot::onShipDied(int shipID)
{
    if ( shipID == _targetInfo.ID ) 
    {
        log("[Tima-%d] My target died!!! hahaha", getID());
        _seeking = true;
    } else 
    {
        log("[Tima-%d] Hum.. another dummy died!", getID());
    }
}

void TimaBot::onPowerUpSpotted()
{
    log("5-[Tima-%d]: Hey, theres a powerup over there!", getID());
}

void TimaBot::onHitByOtherShip(float power, float damageInflicted, float missileAngle)
{
   log("6-[Tima-%d]: A ship hit me!!", getID());
}

void TimaBot::onDeath()
{
    log("7-[Tima-%d]: I just died x_x", getID());
}