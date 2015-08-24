#ifndef __MADSHOOTER_HUDLAYER_HPP__
#define __MADSHOOTER_HUDLAYER_HPP__

#include "Game.hpp"

class HUDLayer : public cocos2d::Layer
{
private:
    cocos2d::Label* lblTitle;
public:
    virtual ~HUDLayer();
    virtual bool init() override;
    virtual void update(float dt) override;
    CREATE_FUNC(HUDLayer);
};

#endif