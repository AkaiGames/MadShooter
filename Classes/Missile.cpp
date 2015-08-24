#include "Missile.hpp"

USING_NS_CC;
using namespace CocosDenshion;

Missile::~Missile()
{
    //log("~Missile(T=%d,ID=%d)", getType(), getID());
}

Missile::Missile(BeamType t)
{
    _beamType = t;
    Color4F();
}

Missile* Missile::create(BeamType t)
{
    Missile *pRet = new(std::nothrow) Missile(t);
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

bool Missile::init()
{
    if ( !Entity::init() ){
        return false;
    } 
    
    setRotation(0);
    std::ostringstream stream;
    stream << "lasers/L" << ((int)_beamType) << "1.png";
    setTexture(stream.str());
    
    // [[-- sets the physic body
    auto physicBody = PhysicsBody::createBox( Size( getContentSize().width , getContentSize().height ) ,
                                              PhysicsMaterial(0, 0.02, 0) );
    physicBody->setCategoryBitmask(Type::MISSILE);
    //physicBody->setCollisionBitmask(ENTITY_SHIP | ENTITY_MISSILE | ENTITY_BORDER);
    physicBody->setContactTestBitmask(Type::SHIP | Type::MISSILE | Type::BORDER);
    physicBody->setTag(Type::MISSILE);
    setPhysicsBody( physicBody );
    // --]]
    
    _type = Type::MISSILE;
    
    setVisible(true);
    
    // Tells Cocos2d-x to call the Node's update function
    scheduleUpdate();
    
    return true;
}

void Missile::setTarget(int shipID)
{
    this->_targetShipID = shipID;
}

void Missile::update(float dt)
{
    Entity::update(dt);
}


