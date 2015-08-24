#include "PlayerTwo.hpp"

USING_NS_CC;

PlayerTwo::~PlayerTwo()
{
    log("~PlayerTwo()");
}

bool PlayerTwo::init()
{
    if ( !Player::init() ){
        return false;
    }
    
    setEventHandlers();
    
    // Tells Cocos2d-x to call the Node's update function
    scheduleUpdate();
    
    return true;
}

void PlayerTwo::setEventHandlers()
{
    auto keyboardListener = EventListenerKeyboard::create();
    
    keyboardListener->onKeyPressed = [=](EventKeyboard::KeyCode keyCode, Event* event) {
        if ( keyCode == EventKeyboard::KeyCode::KEY_W)
        {
            turnEngine( true ); // engine [on]
        }
        if (keyCode == EventKeyboard::KeyCode::KEY_A)
        {
            _turningDirection = -1; // while holding LEFT ship will be constantly turning LEFT
        }
        if (keyCode == EventKeyboard::KeyCode::KEY_D)
        {
            _turningDirection = 1; // while holding RIGHT ship will be constantly turning RIGHT
        }
    };
    
    keyboardListener->onKeyReleased = [=](EventKeyboard::KeyCode keyCode, Event* event) {
        if (keyCode == EventKeyboard::KeyCode::KEY_F)
        {
            fire();
        }
        if (keyCode == EventKeyboard::KeyCode::KEY_W)
        {
            turnEngine( false ); // engine [off]
        }
        if (keyCode == EventKeyboard::KeyCode::KEY_A)
        {
            _turningDirection = 0;
        }
        if (keyCode == EventKeyboard::KeyCode::KEY_D)
        {
            _turningDirection = 0;
        }
    };
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(keyboardListener, this);
}

void PlayerTwo::update(float dt)
{
    Player::update(dt);
}
