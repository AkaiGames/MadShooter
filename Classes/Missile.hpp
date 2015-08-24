#ifndef __MADSHOOTER_MISSILE_HPP__
#define __MADSHOOTER_MISSILE_HPP__

#include "Game.hpp"
#include "ShipInfo.hpp"
#include "BeamType.hpp"
#include "Entity.hpp"

class Ship;

class Missile : public Entity
{
private:
    Missile(BeamType t);
    int speedFactor;
    int _sourceShipID, _targetShipID;
    BeamType _beamType;
public:
    virtual ~Missile();
    virtual bool init() override;
    virtual void update(float dt) override;
    // TODO: This method may be used in future releases to allow
    // the missile to chase this target while moving.
    void setTarget(int shipID);
    static Missile* create(BeamType t = BeamType::Normal);
    //CREATE_FUNC(Missile);
friend class Ship;
friend class NewGameSinglePlayerScene;
};

#endif
