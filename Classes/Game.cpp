#include "Game.hpp"

USING_NS_CC;
using namespace CocosDenshion;

bool Game::_isPaused = false;
Size Game::VISIBLE_SIZE = Size();
Vec2 Game::VISIBLE_ORIGIN = Vec2();
int Game::GREATER_DIMENSION = 0;
bool Game::debugDraw = false;

float Game::DropRate::Health       = 0.05f;

float Game::DropRate::EnergyNormal = 0.03f;
float Game::DropRate::EnergyIce    = 0.01f;
float Game::DropRate::EnergyFire   = 0.01f;

float Game::DropRate::ShieldBronze = 0.03f;
float Game::DropRate::ShieldSilver = 0.01f;
float Game::DropRate::ShieldGold   = 0.01f;

float Game::DropRate::AmmoBronze   = 0.03f;
float Game::DropRate::AmmoSilver   = 0.01f;
float Game::DropRate::AmmoGold     = 0.01f;

void Game::pause()
{
    if ( _isPaused == false ) { 
        _isPaused = true;
        // pause animations
        Director::getInstance()->stopAnimation();
        // if we are using SimpleAudioEngine, it must be paused
        SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
    }
}

bool Game::isPaused()
{
    return _isPaused;
}

void Game::resume()
{
    if ( _isPaused == true ) {
        // resume animations
        Director::getInstance()->startAnimation();
        // if you use SimpleAudioEngine, it must resume here
        SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
        // unpause
        _isPaused = false;
    }
}
