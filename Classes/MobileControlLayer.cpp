#include "MobileControlLayer.hpp"

USING_NS_CC;

MobileControlLayer::~MobileControlLayer()
{
    btFire->release();
    btAnalog->release();
}

bool MobileControlLayer::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    // [[-- Onscreen controls
    
    int h_offset = 70;
    int v_offset = 50;
    
    btAnalog = ui::Button::create("ui/shadedLightAnalogic.png");
    btAnalog->setPosition( Vec2( btAnalog->getSize().width / 2 + h_offset , 
                               btAnalog->getSize().height / 2 + v_offset ) );
    btAnalog->setAnchorPoint( Vec2(0.5,0.5) );
    btAnalog->setScale(0.8f);
    btAnalog->setOpacity(80);
    btAnalog->retain();
    
    addChild(btAnalog);
    
    btFire = ui::Button::create("ui/shadedLightFire.png");
    btFire->setPosition( Vec2(Game::VISIBLE_SIZE.width - btFire->getSize().width - h_offset, 
                              btFire->getSize().height + v_offset ) );
    btFire->setAnchorPoint( Vec2(0.5,0.5) );
    btFire->setOpacity(80);
    btFire->retain();
    addChild(btFire);
    
    h_offset += 100;
    
    // --]]
    
    return true;
}

void MobileControlLayer::update(float dt)
{
    cocos2d::Node::update(dt);
}


