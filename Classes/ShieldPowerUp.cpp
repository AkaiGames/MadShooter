#include "ShieldPowerUp.hpp"

USING_NS_CC;

ShieldPowerUp::~ShieldPowerUp()
{
    log("~HealthPowerUp(T=%d,ID=%d)", getType(), getID());
}

ShieldPowerUp::ShieldPowerUp(ShieldLevel l)
{
    _level = l;
}

ShieldPowerUp* ShieldPowerUp::create(ShieldLevel l)
{
    ShieldPowerUp *pRet = new(std::nothrow) ShieldPowerUp(l);
    if (pRet && pRet->init() && l != ShieldLevel::NoShield)
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

bool ShieldPowerUp::init() 
{
    if ( !PowerUp::init() ) {
        return false;
    }
    
    std::ostringstream stream;
    stream << "powerups/shield" << (int)(_level) << ".png";
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
    
    _stats.defense = 10;
    
    return true;
}

void ShieldPowerUp::apply(Ship* s)
{
    bool shieldRefresh = false;
    // increases defense
    s->_defense += _stats.defense;
    log("$$$$ Defense of ship %d increased by %d.", s->getID(), _stats.defense);
    
    // and wraps shield with a protection shield
    s->_shieldDefense = s->_defense * ((int)_level);
    log("$$$$ Applying shield level %d to ship %d : %f", (int)_level, s->getID(), s->_shieldDefense ); 
    
    if ( s->hasShield() == true ) {
        std::ostringstream stream;
        stream << "shield-" << s->getID();
        auto shieldSprite = (Sprite*) s->_sprite->getChildByName(stream.str());
        if ( shieldSprite == nullptr ) {
            log("Wops!! No shield, something weird happed in ship %d!", s->getID());
            return;
        }
        shieldSprite->removeFromParentAndCleanup(true);
        log("(((((( Shield refreshing for ship %d...", s->getID() );
        shieldRefresh = true;
    }
    
    s->_shieldLevel = _level;
    
    std::ostringstream stream;
    stream << "ships/shield" << (int)_level << ".png"; 
    auto sprite = Sprite::create(stream.str());
    auto v1 = Vec2(s->getXPos(), s->getYPos());
    auto v2 = s->convertToWorldSpace(v1);
    auto v3 = s->_sprite->convertToNodeSpace(v2);
    sprite->setPosition( v3 );
    std::ostringstream stream2;
    stream2 << "shield-" << s->getID();
    sprite->setName(stream2.str());
    s->_sprite->addChild(sprite, 888);
    FadeIn* fadeIn = nullptr;
    if ( shieldRefresh == false ) {
        fadeIn = FadeIn::create(0.2f);
        sprite->setOpacity(0);
    } else {
        fadeIn = FadeIn::create(0);
    }
    auto delay = DelayTime::create(1000.f);
    auto fadeOut = FadeOut::create(0.2f);
    auto callback1 = CallFunc::create([=](){
        s->_shieldLevel = ShieldLevel::NoShield;
        s->_shieldDefense = 0;
        s->_sprite->removeChild(sprite, true);
        log(">>>> Ship %d vanished!", s->getID());
    });
    auto seq0 = Sequence::create(fadeIn, delay, fadeOut, callback1, nullptr);
    sprite->runAction(seq0);
}

void ShieldPowerUp::update(float dt)
{
    PowerUp::update(dt);
}
