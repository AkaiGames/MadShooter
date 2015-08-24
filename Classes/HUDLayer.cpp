#include "HUDLayer.hpp"

USING_NS_CC;

HUDLayer::~HUDLayer()
{
    lblTitle->release();
}

bool HUDLayer::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    lblTitle = Label::create("MadShooter","fonts/kenvector_future_thin.ttf",15);
    lblTitle->setAnchorPoint(Vec2(0.5,0.5));
    lblTitle->setPosition(3, 400);
    lblTitle->retain();
    addChild(lblTitle);
    
    return true;
}

void HUDLayer::update(float dt)
{
    cocos2d::Node::update(dt);
}
