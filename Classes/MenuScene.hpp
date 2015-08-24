#ifndef __MADSHOOTER_MENUSCENE_HPP__
#define __MADSHOOTER_MENUSCENE_HPP__

#include "Game.hpp"

class MenuScene : public cocos2d::Scene
{
public: 
    MenuScene();
    virtual void onEnter() override;
    //virtual void run() = 0;
};

#endif
