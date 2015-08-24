#ifndef __MADSHOOTER_NEWGAMESINGLEPLAYERSCENE_HPP__
#define __MADSHOOTER_NEWGAMESINGLEPLAYERSCENE_HPP__

#include "Game.hpp"
#include "HUDLayer.hpp"
#include "MobileControlLayer.hpp"
#include "ScriptBot.hpp"
#include "PlayerOne.hpp"
#include "PlayerTwo.hpp"
#include "TimaBot.hpp"
#include "HealthPowerUp.hpp"
#include "EnergyPowerUp.hpp"
#include "ShieldPowerUp.hpp"
#include "DamagePowerUp.hpp"
#include <sstream>
#include <vector>

class NewGameSinglePlayerScene : public cocos2d::Scene
{
private:
    int playerOneID, playerTwoID;
    void scheduleShipDestruction(Ship* ship);
    #if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    /*bool analogPressed;
    HUDLayer* hudLayer;
    MobileControlLayer* controlLayer;*/
    #else
    cocos2d::Label* lblMouseStatus;
    #endif
    float spawnTime;
    float timaSpawn;
    //cocos2d::DrawNode* healthBar; 
public: 
    NewGameSinglePlayerScene();
    virtual ~NewGameSinglePlayerScene();
    virtual void onEnter() override;
    void onAnalogButtonClicked(cocos2d::Ref* pSender);
    void onAnalogButtonMoved(cocos2d::Ref* pSender);
    void onFireButtonClicked(cocos2d::Ref* pSender);
    void onAccelButtonClicked(cocos2d::Ref* pSender);
    void spawnBotAt(const char* filename, int x, int y);
    void toggleDebugDraw();
    bool onContactBegin(cocos2d::PhysicsContact& contact);
    // update schedule
    virtual void update(float dt) override;
    virtual bool init() override;
// for scheduleShipDestruction
friend class Ship;
// for enemies
friend class Missile;
};

#include "Player.hpp"

#endif
