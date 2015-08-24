#ifndef __MADSHOOTER_ENTITY_HPP__
#define __MADSHOOTER_ENTITY_HPP__

#include "Game.hpp"

class Entity : public cocos2d::Sprite
{
public: enum Type { NONE=0, SHIP=1, MISSILE=2, BORDER=4, POWERUP=6 };
private:
    static long int totalEntitiesAlive, nextID;
protected:
    Type _type;
public:
    static int getTotalEntitiesAlive();
    static bool isAlive(std::string entityName);
    static bool isAlive(int entityID);
    static Node* getEntity(std::string entityName);
    static Node* getEntity(int entityID);
    int getID() const;
    Type getType() const;
    virtual ~Entity();
    virtual bool init() override;
    virtual void update(float dt) override;
};

#endif
