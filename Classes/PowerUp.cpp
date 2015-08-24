#include "PowerUp.hpp"

USING_NS_CC;

PowerUp::~PowerUp()
{
    log("~PowerUp(T=%d,ID=%d)", getType(), getID());
}

bool PowerUp::init() 
{
    if ( !Entity::init() ){
        return false;
    }
    
    // foo
    
    return true;
}

void PowerUp::update(float dt)
{
    Entity::update(dt);
}
