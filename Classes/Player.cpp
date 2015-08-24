#include "Player.hpp"

USING_NS_CC;

Player::~Player() {}

bool Player::init()
{
    if ( !Ship::init() ){
        return false;
    }
    
    _turningDirection = 0;
    
    return true;
}

void Player::update(float dt)
{
    Ship::update(dt);
    
    if ( _turningDirection == -1 )
        turnShip( -2 );
    if ( _turningDirection == 1 )
        turnShip( 2 );
}

void Player::onShipStateChanged(ShipInfo info)
{
    log("1-[Ship-%d]: Ship-%d changed.", getID(), info.ID);
}

void Player::onShipOutOfLineOfSight(ShipInfo info)
{
    log("2-[Ship-%d]: I cannot see Ship-%d anymore!", getID(), info.ID);
}

void Player::onShipEnteredLineOfSight(ShipInfo info)
{
    log("3-[Ship-%d]: Ship-%d entered my line of sight!", getID(), info.ID);
}

void Player::onShipDied(int shipID)
{
    log("4-[Ship-%d]: Ship-%d died in front of me!", getID(), shipID);
}

void Player::onPowerUpSpotted()
{
    log("5-[Ship-%d]: Hey, theres a powerup over there!", getID());
}

void Player::onHitByOtherShip(float power, float damageInflicted, float missileAngle)
{
    log("6-[Ship-%d]: A ship hit me!!", getID());
}

void Player::onDeath()
{
    log("7-[Ship-%d]: I just died x_x", getID());
}

