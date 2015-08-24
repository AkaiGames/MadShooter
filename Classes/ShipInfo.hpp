#ifndef __MADSHOOTER_SHIP_INFO_HPP__
#define __MADSHOOTER_SHIP_INFO_HPP__

#include "Game.hpp"
#include "BeamType.hpp"

struct ShipInfo {
    int ID;
    int totalScore, damage;
    bool engineState;
    float angleInDegrees;
    int energyPoints;
    int healthPoints;
    int defense;
    BeamType beamType;
    cocos2d::Vec2 velocity, position;
};

#endif