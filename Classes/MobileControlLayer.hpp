#ifndef __MADSHOOTER_MOBILECONTROLLAYER_HPP__
#define __MADSHOOTER_MOBILECONTROLLAYER_HPP__

#include "Game.hpp"

class MobileControlLayer : public cocos2d::Layer
{
private:
    cocos2d::ui::Button *btAnalog, *btFire;
public:
    virtual ~MobileControlLayer();
    virtual bool init() override;
    virtual void update(float dt) override;
    CREATE_FUNC(MobileControlLayer);
};

#endif