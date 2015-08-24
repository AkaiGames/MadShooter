#ifndef __MADSHOOTER_GAME_HPP__
#define __MADSHOOTER_GAME_HPP__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "audio/include/SimpleAudioEngine.h"

extern "C" {
# include "lua/lua.h"
# include "lua/lauxlib.h"
# include "lua/lualib.h"
}

#include "LuaBridge/LuaBridge.h"

class Game {
private:
    static bool _isPaused;
public:
    // game attributes
    static cocos2d::Size VISIBLE_SIZE;
    static cocos2d::Vec2 VISIBLE_ORIGIN;
    static int GREATER_DIMENSION; // width or height
    static bool debugDraw; // is debugdraw enabled?
    // powerups drop rate
    static struct DropRate {
        static float Health,
                     EnergyNormal,
                     EnergyIce,
                     EnergyFire,
                     ShieldBronze,
                     ShieldSilver,
                     ShieldGold,
                     AmmoBronze,
                     AmmoSilver,
                     AmmoGold,
                     Nothing;
    } DropRate;
    
    // functions
    static bool isPaused();
    static void pause();
    static void resume();
};

#endif