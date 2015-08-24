#include "DamagePowerUp.hpp"

USING_NS_CC;

DamagePowerUp::~DamagePowerUp()
{
    log("~HealthPowerUp(T=%d,ID=%d)", getType(), getID());
}

DamagePowerUp::DamagePowerUp(DamageLevel l)
{
    _damageLevel = l;
}

DamagePowerUp* DamagePowerUp::create(DamageLevel l)
{
    DamagePowerUp *pRet = new(std::nothrow) DamagePowerUp(l);
    if (pRet && pRet->init() && l != DamageLevel::Zero)
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

bool DamagePowerUp::init() 
{
    if ( !PowerUp::init() ){
        return false;
    }
    
    std::ostringstream stream;
    stream << "powerups/DP" << ((int)_damageLevel) << ".png";
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
    
    _stats.damage = 5;
    
    // Tells Cocos2d-x to call the Node's update function
    scheduleUpdate();
    
    return true;
}

void DamagePowerUp::apply(Ship* s)
{
    log("Damage of ship %d increased by: %d", s->getID(), _stats.damage * ((int)_damageLevel) );
    s->_damage += _stats.damage * ((int)_damageLevel);
}

void DamagePowerUp::update(float dt)
{
    PowerUp::update(dt);
}
