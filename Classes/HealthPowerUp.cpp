#include "HealthPowerUp.hpp"

USING_NS_CC;

HealthPowerUp::~HealthPowerUp()
{
    log("~HealthPowerUp(T=%d,ID=%d)", getType(), getID());
}

bool HealthPowerUp::init() 
{
    if ( !PowerUp::init() ){
        return false;
    }
    
    setTexture("powerups/E4.png");
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
    
    _stats.healthPoints = 25;
    
    // Tells Cocos2d-x to call the Node's update function
    scheduleUpdate();
    
    return true;
}

void HealthPowerUp::apply(Ship* s)
{
    s->setHP(s->getHP() + _stats.healthPoints);
    
    if ( s->isOnFire() == true ) {
        log("(_^_^_) Ship %d healed from fire!!!", s->getID() );
        s->_sprite->removeAllChildrenWithCleanup(true);
        s->enable();
        s->_onFire = false;
    }
}

void HealthPowerUp::update(float dt)
{
    PowerUp::update(dt);
}
