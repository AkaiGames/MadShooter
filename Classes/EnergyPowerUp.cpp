#include "EnergyPowerUp.hpp"

USING_NS_CC;

EnergyPowerUp::~EnergyPowerUp()
{
    log("~HealthPowerUp(T=%d,ID=%d)", getType(), getID());
}

EnergyPowerUp::EnergyPowerUp(BeamType t)
{
    _beamType = t;
}

EnergyPowerUp* EnergyPowerUp::create(BeamType t)
{
    EnergyPowerUp *pRet = new(std::nothrow) EnergyPowerUp(t);
    if (pRet && pRet->init())
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = NULL;
        return NULL;
    }
}

bool EnergyPowerUp::init() 
{
    if ( !PowerUp::init() ){
        return false;
    }
    
    std::ostringstream stream;
    stream << "powerups/E" << ((int)_beamType) << ".png";
    setTexture(stream.str());
    setOpacity(0);
    auto fadeIn = FadeIn::create(1.0f);
    runAction(fadeIn);
    
    // [[-- sets the physic body
    auto physicBody = PhysicsBody::createBox( Size( getContentSize().width , getContentSize().height ) ,
                                              PhysicsMaterial(0, 0.02, 0) );
    physicBody->setCategoryBitmask(Type::POWERUP);
    physicBody->setCollisionBitmask( Entity::Type::SHIP );
    physicBody->setContactTestBitmask(Type::SHIP);
    
    //physicBody->setTag();
    setPhysicsBody( physicBody );
    // --]]
    
    _stats.energyPoints = 25;
    
    // Tells Cocos2d-x to call the Node's update function
    scheduleUpdate();
    
    return true;
}

void EnergyPowerUp::apply(Ship* s)
{
    if ( _beamType == BeamType::Ice || _beamType == BeamType::Fire ) {
        s->_beamType = _beamType;
        log("Begin of effect on ship(%d).", s->getID());
        auto delay = DelayTime::create(5.f); // fire and ice will take only 5 seconds.
        auto func = CallFunc::create([=]() {
            s->_beamType = BeamType::Normal;
            // feature #1
            s->setEnergy( s->getEnergy() ); // just to refresh energy color back to normal
            log("End of effect on ship(%d).", s->getID());
        });
        auto seq = Sequence::create(delay, func, nullptr);
        s->_sprite->runAction(seq);
    }
    s->setEnergy(s->getEnergy() + _stats.energyPoints);
}

void EnergyPowerUp::update(float dt)
{
    PowerUp::update(dt);
}
