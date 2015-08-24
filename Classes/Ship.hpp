#ifndef __MADSHOOTER_SHIP_HPP__
#define __MADSHOOTER_SHIP_HPP__

#include "Game.hpp"
#include "Missile.hpp"
#include "Entity.hpp"
#include "ShieldLevel.hpp"
#include <sstream>
#include <math.h>
#include <map>
#include <algorithm>

class Ship : public Entity
{
private:
    // [[-- Player Registration data
    bool _isBot;
    // --]]
    BeamType _beamType;
    // bullets shot by this ship, when a ship is destroyed all these bullets receive all stats of
    // the destroyed ship and get their source pointer nulled. This way they may know that they are orphans
    // and they need to consult shipInfo var to get the last attributes of their parent ships.
    std::list<Missile*> _bullets;
    // [[-- Ray casting and collision info. show raycast used to show ship's line of sight
    typedef std::map<int, bool> LineOfSight;
    LineOfSight _inLineOfSight; // shapes currently in line of sight
    bool _showRayCast;
    cocos2d::DrawNode* _rayNode;
    // --]]
    bool _playingLaser;
    bool _enabled;
    void setParentScene(); // ATTENTION: This MUST be called right after adding this ship as child of a parent container.
    cocos2d::Label* _lblShipName;
    // [[-- Attributes
    int _totalScore;
    int _damage;
    int _defense;
    bool _engineState; // on(true)/off(false)
    // --]]
    float _angleInDegrees;
    // [[-- Health is measured in %
    cocos2d::DrawNode* _healthBar;
    int _healthPoints;
    cocos2d::Vec2 _healthBarVerts[4];
    // --]]
    // [[-- Energy is measured in %
    cocos2d::DrawNode* _energyBar;
    int _energyPoints;
    cocos2d::Vec2 _energyBarVerts[4];
    // --]]
    // [[-- Parent methods used by the bullets
    static void hitShip(ShipInfo actorInfo, Ship* ship);
    // --]]
    bool _targetPositionChanged;
    // other private methods:
    void moveTo( cocos2d::Vec2 pos );
    void updateLabels(); // updates ships labels after moving or physics events
    void force(cocos2d::Vec2 vec);
    void setVelocity(const cocos2d::Vec2& vel);
    // [[-- Ship turning vars
    bool _isTurning;
    float _targetTurningAngle;
    float _totalAngleTurned;
    // --]]
    // [[-- Heat
    float _gunHeat;
    // --]]
    bool _frozen, _onFire;
    ShieldLevel _shieldLevel;
    float _dot; // damage over frame inflicting this ship
    float _shieldDefense; // how much 'life' this shield still has, each time it absorbs damage, this value 
    // is decreased by the damage, when it reaches zero, the shield is canceled.
    // [[-- Frozen or OnFire or Shield
    static bool checkIfShipDied(Ship* ship);
    void enable();
    void disable();
    void setHP(int healthPoints); // used by the bullets
    void setEnergy(int energyPoints); // used by the bullets
    cocos2d::Sprite* _sprite;
protected:
    // [[-- Vectors used to direct the ship
    cocos2d::Vec2 _aimVector;
    cocos2d::Point _targetPosition;
    // --]]
    // other protected methods:
    void setShipName(std::string name); // used only when creating the ship
    cocos2d::Vec2 getVelocity() const;
    void turnEngine(bool state); // turns engine on/off
    void toggleEngine(); // switchs between on/off
    bool getEngineState() const; // checks if ship's engine is on or off
public:
    void registerPlayer(std::string playerName);
    // [[-- CONSTANTS
    static const int MAX_ENERGY = 100;
    static const int MAX_HEALTH = 100;
    // --]]
    bool isBot() const;
    bool onContactBegin(cocos2d::PhysicsContact& contact);
    bool rayCastCallback(cocos2d::PhysicsWorld& world, const cocos2d::PhysicsRayCastInfo& info, void* data);
    virtual ~Ship();
    virtual bool init() override;
    virtual void update(float dt) override;
    // get X position, remember that the anchor point is 0.5
    int getXPos() const;
    int getYPos() const;
    bool isEnabled() const;
    ShipInfo getInfo() const;
    void turnShip( float angle );
    void stopTurning();
    bool isTurning() const;
    int getHP() const;
    int getEnergy() const;
    float getDefense() const;
    // gets the heading angle of the ship in degrees, i.e. where the ship is pointing to.
    float getHeading() const;
    std::string getShipName() const;                                                                                                                                                                                                                        
    void fire();
    int getDamage() const;
    float getRadius() const;
    // [[--
    bool isOnFire() const;
    bool isFrozen() const;
    bool hasShield() const;
    // --]]
    // [[--- PUBLIC SHIP CALLBACK INTERFACE
    virtual void onShipEnteredLineOfSight(ShipInfo info) = 0; // Called when a ship enters my line of sight
    virtual void onShipStateChanged(ShipInfo info) = 0; // Called when a ship changes its state and it's in my line of sight
    virtual void onShipOutOfLineOfSight(ShipInfo info) = 0; // Called when a ship runs away from the line of sight
    virtual void onShipDied(int shipID) = 0; // Called when a ship in my line of sight dies
    virtual void onDeath() = 0; // Called when this ship dies
    virtual void onHitByOtherShip(float power, float damageInflicted, float missileAngle) = 0;
    virtual void onPowerUpSpotted() = 0;
    // --]]
// only bullets can access, destroy or damage ships
friend class Missile;
// for setParentScene();
friend class NewGameSinglePlayerScene;
// for changing attributes
friend class HealthPowerUp;
friend class EnergyPowerUp;
friend class ShieldPowerUp;
friend class DamagePowerUp;
// maximum speed
    static const int MAX_SPEED = 15;
};

#endif
