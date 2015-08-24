#include "ScriptBot.hpp"

USING_NS_CC;
using namespace luabridge;

ScriptBot::~ScriptBot()
{
    log("~ScriptBot(T=%d,ID=%d)", getType(), getID());
}

bool ScriptBot::init()
{
    if ( Ship::init() == false )
        return false;
    
    L = NULL;    
    didTheJob = false;
}

bool ScriptBot::loadScript(std::string filename)
{
    if ( L != nullptr )
        return false;
    
    scriptFilename = filename;
    
    // load script
    L = luaL_newstate();
    if ( luaL_dofile(L, FileUtils::getInstance()->fullPathForFilename(filename.c_str()).c_str() ) ) {
        log("File not found!");
        return (_scriptLoaded = false);
    }

    setShipName(filename.c_str());
    luaL_openlibs(L);
    lua_pcall(L, 0, 0, 0);
    
    // Tells Cocos2d-x to call the Node's update function
    scheduleUpdate();
    
    return (_scriptLoaded = true);
}


void ScriptBot::update(float dt)
{
    Ship::update(dt);
    
    if ( _scriptLoaded == true )
    {
        if (didTheJob == false) {
            turnEngine(true);
            didTheJob = true;
        }
        
        LuaRef func = getGlobal(L, "speak");
        std::string msg = func();
        log("[%s]: %s", scriptFilename.c_str(), msg.c_str());
        
        //fire();
        //turnShip(5);
    } else {
        log("Invalid bot won't move");
    }
}

void ScriptBot::onShipStateChanged(ShipInfo info)
{
    log("1-[Ship-%d]: Ship-%d changed.", getID(), info.ID);
}

void ScriptBot::onShipOutOfLineOfSight(ShipInfo info)
{
    log("2-[Ship-%d]: I cannot see Ship-%d anymore!", getID(), info.ID);
}

void ScriptBot::onShipEnteredLineOfSight(ShipInfo info)
{
    log("3-[Ship-%d]: Ship-%d entered my line of sight!", getID(), info.ID);
}

void ScriptBot::onShipDied(int shipID)
{
    log("4-[Ship-%d]: Ship-%d died in front of me!", getID(), shipID);
}

void ScriptBot::onPowerUpSpotted()
{
    log("5-[Ship-%d]: Hey, theres a powerup over there!", getID());
}

void ScriptBot::onHitByOtherShip(float power, float damageInflicted, float missileAngle)
{
    log("6-[Ship-%d]: A ship hit me!!", getID());
}

void ScriptBot::onDeath()
{
    log("7-[Ship-%d]: I just died x_x", getID());
}
