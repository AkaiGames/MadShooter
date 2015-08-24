#include "Entity.hpp"

USING_NS_CC;

long int Entity::totalEntitiesAlive = 0;
long int Entity::nextID = 1;

Entity::~Entity()
{
    //log("~Entity(T=%d,ID=%d)", getType(), getID());
    totalEntitiesAlive--;
}

int Entity::getID() const
{
    return getTag();
}

Entity::Type Entity::getType() const
{
    return _type;
}

int Entity::getTotalEntitiesAlive()
{
    return totalEntitiesAlive;
}

bool Entity::isAlive(std::string entityName)
{
    return Director::getInstance()->getRunningScene()->getChildByName( entityName ) != nullptr;
}

bool Entity::isAlive(int entityID)
{
    return Director::getInstance()->getRunningScene()->getChildByTag( entityID ) != nullptr;
}

Node* Entity::getEntity(int entityID)
{
    return Director::getInstance()->getRunningScene()->getChildByTag( entityID );
}

Node* Entity::getEntity(std::string entityName)
{
    return Director::getInstance()->getRunningScene()->getChildByName( entityName );
}

bool Entity::init()
{
    if ( !Sprite::init() ){
        return false;
    }
    
    // set initial entity type, not assiged yet.
    _type = Type::NONE;
    // assures that we dont create a entity with an existing id
    while ( isAlive( nextID ) == true )
        nextID++;
    setTag(nextID);
    totalEntitiesAlive++;
    
    return true;
}


void Entity::update(float dt)
{
    Node::update(dt);
}


