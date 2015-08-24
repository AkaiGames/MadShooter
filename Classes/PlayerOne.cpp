#include "PlayerOne.hpp"

USING_NS_CC;

PlayerOne::~PlayerOne()
{
    log("~PlayerOne()");
}

bool PlayerOne::init()
{
    if ( !Player::init() ){
        return false;
    }
    
    setEventHandlers();
    
    // Tells Cocos2d-x to call the Node's update function
    scheduleUpdate();
    
    return true;
}

void PlayerOne::setEventHandlers()
{
    auto keyboardListener = EventListenerKeyboard::create();
    
    keyboardListener->onKeyPressed = [=](EventKeyboard::KeyCode keyCode, Event* event) {
        if ( keyCode == EventKeyboard::KeyCode::KEY_UP_ARROW )
        {
            turnEngine( true ); // engine [on]
        }
        if (keyCode == EventKeyboard::KeyCode::KEY_LEFT_ARROW)
        {
            _turningDirection = -1; // while holding LEFT ship will be constantly turning LEFT
        }
        if (keyCode == EventKeyboard::KeyCode::KEY_RIGHT_ARROW)
        {
            _turningDirection = 1; // while holding RIGHT ship will be constantly turning RIGHT
        }
    };
    
    keyboardListener->onKeyReleased = [=](EventKeyboard::KeyCode keyCode, Event* event) {
        if (keyCode == EventKeyboard::KeyCode::KEY_SPACE)
        {
            fire();
        }
        if (keyCode == EventKeyboard::KeyCode::KEY_UP_ARROW)
        {
            turnEngine( false ); // engine [off]
        }
        if (keyCode == EventKeyboard::KeyCode::KEY_LEFT_ARROW)
        {
            _turningDirection = 0;
        }
        if (keyCode == EventKeyboard::KeyCode::KEY_RIGHT_ARROW)
        {
            _turningDirection = 0;
        }
    };
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(keyboardListener, this);
}

void PlayerOne::update(float dt)
{
    Player::update(dt);
}
