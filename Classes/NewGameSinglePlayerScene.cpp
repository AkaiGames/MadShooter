#include "NewGameSinglePlayerScene.hpp"
#include "Entity.hpp"
#include <lobject.h>
#include <signal.h>

USING_NS_CC;
using namespace CocosDenshion;

NewGameSinglePlayerScene::NewGameSinglePlayerScene()
{
    Scene::init();
    #if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    //analogPressed = false;
    #endif
}

NewGameSinglePlayerScene::~NewGameSinglePlayerScene() {
    #if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    /*
    btAnalog->release();*/
    #endif
}

void NewGameSinglePlayerScene:: spawnBotAt(const char* filename, int x, int y) 
{
    std::stringstream fullpath;
    fullpath << "bots/" << filename;
    auto enemy = ScriptBot::create();
    addChild(enemy);
    enemy->loadScript(fullpath.str().c_str());
    enemy->moveTo( Vec2(x, y) );
    
    setVisible(true);
    
    log("New enemy ship with id: %d", enemy->getID()); 
}

void NewGameSinglePlayerScene::scheduleShipDestruction(Ship* ship)
{
    log("InShip HP: %d", ship->getHP());
    //enemiesToDestroy.pushBack(ship);
    log("InShip HP: %d", ship->getHP());
    //enemies.eraseObject(ship);
    log("InShip HP: %d", ship->getHP());
}

bool NewGameSinglePlayerScene::init()
{
    if ( !Scene::initWithPhysics() )
    {
        return false;
    }
    
    log("initWithPhysics()");
    
    return cocos2d::Scene::init();
}

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
/*
void NewGameSinglePlayerScene::onAnalogButtonClicked(Ref* pSender)
{

}

void NewGameSinglePlayerScene::onAnalogButtonMoved(Ref* pSender)
{

}

void NewGameSinglePlayerScene::onFireButtonClicked(Ref* pSender)
{
    
}

void NewGameSinglePlayerScene::onAccelButtonClicked(Ref* pSender)
{

}*/
#endif

void NewGameSinglePlayerScene::onEnter()
{
    Scene::onEnter();
    
    if ( Scene::initWithPhysics() == false )
    {
        log("initWithPhysics == false");
        return;
    }
    
    // [[-- DEBUG
    
    auto edgeBody = PhysicsBody::createEdgeBox( Game::VISIBLE_SIZE, PHYSICSBODY_MATERIAL_DEFAULT, 3 );
    edgeBody->setCategoryBitmask(Entity::Type::BORDER);
    //edgeBody->setCollisionBitmask(Entity::Type::MISSILE | Entity::Type::SHIP );
    edgeBody->setContactTestBitmask(Entity::Type::SHIP | Entity::Type::MISSILE );
    
    auto edgeNode = Node::create();
    edgeNode->setPosition( Vec2( Game::VISIBLE_SIZE.width / 2 + Game::VISIBLE_ORIGIN.x,
                                 Game::VISIBLE_SIZE.height / 2 + Game::VISIBLE_ORIGIN.y ) );
    edgeNode->setPhysicsBody( edgeBody );
    edgeNode->setTag(0);
    
    addChild( edgeNode );
    // --]]
    
    // [[-- Register Collision Callback function that will be called for collision of nodes
    
    auto contactListener = EventListenerPhysicsContact::create();
    contactListener->onContactBegin = CC_CALLBACK_1(NewGameSinglePlayerScene::onContactBegin, this);
    getEventDispatcher()->addEventListenerWithSceneGraphPriority(contactListener, this);
    
    // --]]
    
    // configure physic world
    
    getPhysicsWorld()->setGravity( Vec2( 0, 0 ) );
    
    // [[-- Background image
    
    auto backgroundSprite = Sprite::create("background.png");
    
    backgroundSprite->setAnchorPoint(Vec2(0.5, 0.5));
    backgroundSprite->setPosition(Vec2(Game::VISIBLE_SIZE.width/2 + Game::VISIBLE_ORIGIN.x, 
                                       Game::VISIBLE_SIZE.height/2 + Game::VISIBLE_ORIGIN.y));
    
    addChild(backgroundSprite, 0);
    
    auto backgroundShadowSprite = Sprite::create("background_shadow.png");
    
    backgroundShadowSprite->setAnchorPoint(Vec2(0.5, 0.5));
    backgroundShadowSprite->setPosition(Vec2(Game::VISIBLE_SIZE.width/2 + Game::VISIBLE_ORIGIN.x, 
                                             Game::VISIBLE_SIZE.height/2 + Game::VISIBLE_ORIGIN.y));
    //backgroundSprite->runAction(Liquid::create(3, Size(16,12), 4, 20));
    
    this->addChild(backgroundShadowSprite, 555);
    
    // --]]
    
    // [[-- HUD Layer
    
    #if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    /*
    hudLayer = HUDLayer::create();
    hudLayer->retain();
    addChild(hudLayer, 777);
    
    // --]]
    
    // [[-- Controls Layer
    
    controlLayer = MobileControlLayer::create();
    addChild(controlLayer, 777);
    
    // Mobile stuff
    
    // [[-- Mobile Touch Events
    /*
    auto touchListener = EventListenerTouchOneByOne::create();
    
    touchListener->setSwallowTouches(true);
    
    touchListener->onTouchBegan = [](Touch* touch, Event* event) {
        return true;
    };
    
    touchListener->onTouchMoved = [](Touch* touch, Event* event) {
        // pass
    };
    
    touchListener->onTouchEnded = [=](Touch* touch, Event* event) {
        std::stringstream stream;
        stream << touch->getLocation().x << " " << touch->getLocation().y;
    };
    
    //_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, btFire);
    _eventDispatcher->addEventListenerWithFixedPriority(touchListener, 777);
    */
    // --]]
    #endif
    
    // --]]
    
    // [[-- null player ships
    
    playerOneID = -2;
    playerTwoID = -2;
    
    // create a custom event to notify when the ship dies, this is used to nullify the pointer so we
    // may spawn playerone again later.
    /*auto listener = EventListenerCustom::create("player_died", [=](EventCustom* event){
        int* id = static_cast<int*>(event->getUserData());
        
        if ( (*id) == playerOne->getID() ) {
            log("It was playerOne!");
            playerOne->release();
            playerOne = nullptr;
        } else if ( (*id) == playerTwo->getID() ) {
            log("It was playerTwo!");
            playerTwo->release();
            playerTwo = nullptr;
        } else {
            log("It was an unkown ship: %d!", (*id));
        }
        
        delete id;
    });
    
    _eventDispatcher->addEventListenerWithFixedPriority(listener, 1);*/
    
    // --]]

    // [[-- Event listeners
    
    auto keyboardListener = EventListenerKeyboard::create();
    keyboardListener->onKeyReleased = [=](EventKeyboard::KeyCode keyCode, Event* event) {
        // spacebar
        if (keyCode == EventKeyboard::KeyCode::KEY_Z) {
            int x = random<int>(50, Game::VISIBLE_SIZE.width-50);
            int y = random<int>(50, Game::VISIBLE_SIZE.height-50);
            spawnBotAt("AVA.lua", x, y);
        } else if (keyCode == EventKeyboard::KeyCode::KEY_X) {
            int x = random<int>(50, Game::VISIBLE_SIZE.width-50);
            int y = random<int>(50, Game::VISIBLE_SIZE.height-50);
            spawnBotAt("BMO.lua", x, y);
        } else if (keyCode == EventKeyboard::KeyCode::KEY_C) {
            int x = random<int>(50, Game::VISIBLE_SIZE.width-50);
            int y = random<int>(50, Game::VISIBLE_SIZE.height-50);
            spawnBotAt("Nptr.lua", x, y);
        } else if (keyCode == EventKeyboard::KeyCode::KEY_V) {
            int x = random<int>(50, Game::VISIBLE_SIZE.width-50);
            int y = random<int>(50, Game::VISIBLE_SIZE.height-50);
            spawnBotAt("Tima.lua", x, y);
        } else if (keyCode == EventKeyboard::KeyCode::KEY_T) {
            int x = random<int>(50, Game::VISIBLE_SIZE.width-50);
            int y = random<int>(50, Game::VISIBLE_SIZE.height-50);
            auto tima = TimaBot::create();
            addChild(tima);
            tima->setShipName("Tima");
            tima->moveTo( Vec2(x, y) );
            log("New Tima born with id: %d", tima->getID()); 
        } else if (keyCode == EventKeyboard::KeyCode::KEY_1) {
            auto hpUp = HealthPowerUp::create();
            int x = random<int>(50, Game::VISIBLE_SIZE.width-50);
            int y = random<int>(50, Game::VISIBLE_SIZE.height-50);
            hpUp->setPosition(x, y);
            hpUp->setAnchorPoint( Vec2(0.5, 0.5) );
            addChild(hpUp);
        } else if (keyCode == EventKeyboard::KeyCode::KEY_2) {
            auto energy = EnergyPowerUp::create(BeamType::Normal);
            int x = random<int>(50, Game::VISIBLE_SIZE.width-50);
            int y = random<int>(50, Game::VISIBLE_SIZE.height-50);
            energy->setPosition(x, y);
            energy->setAnchorPoint( Vec2(0.5, 0.5) );
            addChild(energy);
        } else if (keyCode == EventKeyboard::KeyCode::KEY_3) {
            auto energy = EnergyPowerUp::create(BeamType::Ice);
            int x = random<int>(50, Game::VISIBLE_SIZE.width-50);
            int y = random<int>(50, Game::VISIBLE_SIZE.height-50);
            energy->setPosition(x, y);
            energy->setAnchorPoint( Vec2(0.5, 0.5) );
            addChild(energy);
        } else if (keyCode == EventKeyboard::KeyCode::KEY_4) {
            auto energy = EnergyPowerUp::create(BeamType::Fire);
            int x = random<int>(50, Game::VISIBLE_SIZE.width-50);
            int y = random<int>(50, Game::VISIBLE_SIZE.height-50);
            energy->setPosition(x, y);
            energy->setAnchorPoint( Vec2(0.5, 0.5) );
            addChild(energy);
        } else if (keyCode == EventKeyboard::KeyCode::KEY_5) {
            auto shield = ShieldPowerUp::create(ShieldLevel::Low);
            int x = random<int>(50, Game::VISIBLE_SIZE.width-50);
            int y = random<int>(50, Game::VISIBLE_SIZE.height-50);
            shield->setPosition(x, y);
            shield->setAnchorPoint( Vec2(0.5, 0.5) );
            addChild(shield);
        } else if (keyCode == EventKeyboard::KeyCode::KEY_6) {
            auto shield = ShieldPowerUp::create(ShieldLevel::Medium);
            int x = random<int>(50, Game::VISIBLE_SIZE.width-50);
            int y = random<int>(50, Game::VISIBLE_SIZE.height-50);
            shield->setPosition(x, y);
            shield->setAnchorPoint( Vec2(0.5, 0.5) );
            addChild(shield);
        } else if (keyCode == EventKeyboard::KeyCode::KEY_7) {
            auto shield = ShieldPowerUp::create(ShieldLevel::High);
            int x = random<int>(50, Game::VISIBLE_SIZE.width-50);
            int y = random<int>(50, Game::VISIBLE_SIZE.height-50);
            shield->setPosition(x, y);
            shield->setAnchorPoint( Vec2(0.5, 0.5) );
            addChild(shield);
        } else if (keyCode == EventKeyboard::KeyCode::KEY_8) {
            auto power = DamagePowerUp::create(DamageLevel::Single);
            int x = random<int>(50, Game::VISIBLE_SIZE.width-50);
            int y = random<int>(50, Game::VISIBLE_SIZE.height-50);
            power->setPosition(x, y);
            power->setAnchorPoint( Vec2(0.5, 0.5) );
            addChild(power);
        } else if (keyCode == EventKeyboard::KeyCode::KEY_9) {
            auto power = DamagePowerUp::create(DamageLevel::Double);
            int x = random<int>(50, Game::VISIBLE_SIZE.width-50);
            int y = random<int>(50, Game::VISIBLE_SIZE.height-50);
            power->setPosition(x, y);
            power->setAnchorPoint( Vec2(0.5, 0.5) );
            addChild(power);
        } else if (keyCode == EventKeyboard::KeyCode::KEY_0) {
            auto power = DamagePowerUp::create(DamageLevel::Triple);
            int x = random<int>(50, Game::VISIBLE_SIZE.width-50);
            int y = random<int>(50, Game::VISIBLE_SIZE.height-50);
            power->setPosition(x, y);
            power->setAnchorPoint( Vec2(0.5, 0.5) );
            addChild(power);
        } else if (keyCode == EventKeyboard::KeyCode::KEY_CTRL) {
            //if (enemies.size() == 0 ) return;
            //Ship* en = enemies.back();
            //log("Removing enemy ship at %dx%d", en->getXPos(), en->getYPos());
            //removeEnemyFromGame(en);
        } else if( keyCode == EventKeyboard::KeyCode::KEY_F1 ) {
            //log("Number of enemy ships: %d", enemies.size());
        } else if (  keyCode == EventKeyboard::KeyCode::KEY_F2 ) {
            toggleDebugDraw();
        // spawn playerOne
        } else if( keyCode == EventKeyboard::KeyCode::KEY_KP_ENTER) {
            if ( Entity::isAlive( "player_one" ) == false ) {
                Player* player = PlayerOne::create();
                player->registerPlayer("Player 1");
                player->setName("player_one");
                addChild(player);
            } else {
                log("Cannot spawn playerOne, it's alive!");
            }
        // spawn playerTwo
        } else if( keyCode == EventKeyboard::KeyCode::KEY_S ) {
            if ( Entity::isAlive( "player_two" ) == false ) {
                Player* player = PlayerTwo::create();
                player->registerPlayer("Player 2");
                player->setName("player_two");
                addChild(player);
            } else {
                log("Cannot spawn playerTwo, it's alive!");
            }
                
        } else if ( keyCode == EventKeyboard::KeyCode::KEY_P ) {
            if ( Game::isPaused() == false ) {
                auto pausedLabel = Label::createWithTTF("PAUSED","fonts/kenvector_future_thin.ttf", 60);
                pausedLabel->enableOutline(Color4B::BLACK, 1);
                pausedLabel->setPosition( Vec2( Game::VISIBLE_SIZE.width / 2 + Game::VISIBLE_ORIGIN.x,
                                                Game::VISIBLE_SIZE.height / 2 + Game::VISIBLE_ORIGIN.y ) );
                pausedLabel->setName("paused_label");
                pausedLabel->setAnchorPoint( Vec2(0.5,0.5) );
                pausedLabel->setOpacity(0);
                addChild(pausedLabel, 9999);
                auto fade = FadeIn::create(0.1f);
                auto callback = CallFunc::create([=]() {
                    Game::pause();
                });
                auto seq = Sequence::create(fade, callback, nullptr);
                pausedLabel->runAction(seq);
            } else {
                Game::resume();
                //removeChildByName("paused_label");
                Label* pausedLabel = (Label*) Entity::getEntity("paused_label");
                auto fadeOut = FadeOut::create(0.1f);
                auto callback = CallFunc::create([=]() {
                    removeChildByName("paused_label");
                });
                auto seq = Sequence::create(fadeOut, callback, nullptr);
                pausedLabel->runAction(seq);
            }
        }
    };
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(keyboardListener, this);
    
    // --]]
    
    spawnTime = 0;
    
    // Tells Cocos2d-x to call the Node's update function
    this->scheduleUpdate();
}

void NewGameSinglePlayerScene::toggleDebugDraw()
{
    if ( Game::debugDraw == true ) {
        getPhysicsWorld()->setDebugDrawMask( PhysicsWorld::DEBUGDRAW_NONE );
        Game::debugDraw = false;
    }
    else {
        getPhysicsWorld()->setDebugDrawMask( PhysicsWorld::DEBUGDRAW_ALL );
        Game::debugDraw = true;
    }
}

bool NewGameSinglePlayerScene::onContactBegin(PhysicsContact& contact) {
    // My convention: A hits B
    Node *nodeA = contact.getShapeA()->getBody()->getNode(),
         *nodeB = contact.getShapeB()->getBody()->getNode(),
         *tmp = nullptr;
    int tagA = nodeA->getTag(),
        tagB = nodeB->getTag();
    int catA = contact.getShapeA()->getCategoryBitmask(),
        catB = contact.getShapeB()->getCategoryBitmask();
    bool ret = true;
    
    // bullet hit something
    if ( catA == Entity::Type::MISSILE || catB == Entity::Type::MISSILE ) {
        
        // assures that the first node (A) belongs is a bullet
        if ( catA != Entity::Type::MISSILE ) {
            tmp = nodeA;
            nodeA = nodeB;
            nodeB = tmp;
        }
        
        Missile* bullet = (Missile*) nodeA;
        
        tagA = nodeA->getTag();
        tagB = nodeB->getTag();
        catA = nodeA->getPhysicsBody()->getCategoryBitmask();
        catB = nodeB->getPhysicsBody()->getCategoryBitmask();
        
        // remove it from the physic world and schedule sprite removal from its parent
        bullet->removeFromPhysicsWorld();
        // gets the beam type of the ship that shot it
        BeamType bType = 
            (bullet->_sourceShip != nullptr) ? bullet->_sourceShip->_beamType : bullet->_sourceInfo.beamType;
        std::ostringstream stream;
        stream << "lasers/L" << ((int)bType) << "4.png";
        //log("Creating laser: %s for missile: %d", stream.str().c_str(), bullet->getID() );
        bullet->setTexture(stream.str());
        auto fadeOut = FadeOut::create(0.2f);
        auto callFunc = CallFunc::create([=](){
            // if the ship that shot the bullet is still alive, remove this bullet from its list of bullets
            if ( bullet->_sourceShip != nullptr ) {
                //log("Removing this bullet from its parent's list...");
                bullet->_sourceShip->_bullets.remove(bullet);
            }
            bullet->removeFromParentAndCleanup(true);
        });
        auto seq = Sequence::create(fadeOut, callFunc, nullptr);
        bullet->runAction(seq);
        
        if ( catB == Entity::Type::BORDER ) {
            ;//log("[Missile(%d)] hit [Border(%d)]! Bye... :'(", tagA, tagB );
        }
        else if ( catB == Entity::Type::MISSILE ) {
            //log("[Missile(%d)] hit [Missile(%d)]!", tagA, tagB );
            Missile* bullet2 = (Missile*) nodeB;
            
            // remove it from the physic world and schedule sprite removal from its parent
            bullet2->removeFromPhysicsWorld();
            // gets the beam type of the ship that shot it
            BeamType bType2 = 
                (bullet2->_sourceShip != nullptr) ? bullet2->_sourceShip->_beamType : bullet2->_sourceInfo.beamType;
            std::ostringstream stream2;
            //log("Creating laser: %s for missile2: %d", stream2.str().c_str(), bullet2->getID() );
            stream2 << "lasers/L" << ((int)bType2) << "4.png";
            bullet2->setTexture(stream2.str());
            auto fadeOut2 = FadeOut::create(0.2f);
            auto callFunc2 = CallFunc::create([=](){
                // if the ship that shot the bullet is still alive, remove this bullet from its list of bullets
                if ( bullet->_sourceShip != nullptr ) {
                    //log("Removing this bullet from its parent's list...");
                    bullet->_sourceShip->_bullets.remove(bullet);
                }
                bullet2->removeFromParentAndCleanup(true);
            });
            auto seq2 = Sequence::create(fadeOut2, callFunc2, nullptr);
            bullet2->runAction(seq2);
        }
        else if ( catB == Entity::Type::SHIP ) {
            //log("[Missile(%d)] hit [Ship(%d)]!", tagA, tagB );
            // if her parrent did not die yet get her stats (her parent => Ship* , nodeB => Sprite*)
            Ship* s = (Ship*) nodeB->getParent();
            
            if ( bullet->_sourceShip != nullptr ) {
                ShipInfo info = bullet->_sourceShip->getInfo();
                //log("+  before hitShip(); 1");
                Ship::hitShip( info , s );
                //log("+ after hitShip(); 1");
            } else {
                //log("- before hitShip(); 2");
                Ship::hitShip( bullet->_sourceInfo, s );
                //log("- after hitShip(); 2");
            }
            ret = false;
        }
        else {
            //log("[Missile(%d)] hit [unknown(%d)]!", tagA, tagB );
            ret = false;
        }
    // ship hit something
    } else if ( catA == Entity::Type::SHIP || catB == Entity::Type::SHIP ) {
        
        // assures that the first(A) is a ship
        if ( catA != Entity::Type::SHIP ) {
            tmp = nodeA;
            nodeA = nodeB;
            nodeB = tmp;
        }
        
        tagA = nodeA->getTag();
        tagB = nodeB->getTag();
        catA = nodeA->getPhysicsBody()->getCategoryBitmask();
        catB = nodeB->getPhysicsBody()->getCategoryBitmask();
        
        if ( catB == Entity::Type::BORDER )
            ;//log("[Ship(%d)] hit [Border(%d)]!", tagA, tagB );
        else if ( catB == Entity::Type::SHIP )
            ;//log("[Ship(%d)] hit [Ship(%d)]!", tagA, tagB );
        else if ( catB == Entity::Type::POWERUP ) {
            Ship* s = (Ship*) nodeA->getParent();
            PowerUp* up = (PowerUp*) nodeB;
            up->apply(s);
            up->removeFromPhysicsWorld();
            auto fadeOut3 = FadeOut::create(0.2f);
            auto callFunc3 = CallFunc::create([=](){
                up->removeFromParentAndCleanup(true);
            });
            auto seq2 = Sequence::create(fadeOut3, callFunc3, nullptr);
            up->runAction(seq2);
            ret = false; // ship will not 'collide' the powerup, it will 'swallow' it
        }
        else
            ;//log("[Ship(%d)] hit [unknown(%d)]!", tagA, tagB );
    // just for debugging purposes
    } else {
        ;//log("Unknown collision: nodeA(%d) => nodeB(%d)", nodeA->getTag(), nodeB->getTag() );
    }
    
    return ret;
}

void NewGameSinglePlayerScene::update(float dt)
{
    cocos2d::Node::update(dt);
    
    spawnTime += dt;
    
    if ( spawnTime > 1 ) {
        
        spawnTime = 0;
    
        double chance = (double)rand() / RAND_MAX;
        
        int x = random<int>(50, Game::VISIBLE_SIZE.width-50);
        int y = random<int>(50, Game::VISIBLE_SIZE.height-50);
        
        log(" Chance: %f", chance);

        float rate = Game::DropRate::Health;
        if ( chance < rate ) {
            log("____ Health spawned ____");
            auto hpUp = HealthPowerUp::create();
            hpUp->setPosition(x, y);
            hpUp->setAnchorPoint( Vec2(0.5, 0.5) );
            addChild(hpUp);
        }
        else {
            rate += Game::DropRate::EnergyNormal;
            if ( chance < rate ) {
                log("____ EnergyNormal spawned ____");
                auto energy = EnergyPowerUp::create(BeamType::Normal);
                energy->setPosition(x, y);
                energy->setAnchorPoint( Vec2(0.5, 0.5) );
                addChild(energy);
            }
            else {
                rate += Game::DropRate::EnergyIce;
                if ( chance < rate ) {
                    log("____ EnergyIce spawned ____");
                    auto energy = EnergyPowerUp::create(BeamType::Ice);
                    energy->setPosition(x, y);
                    energy->setAnchorPoint( Vec2(0.5, 0.5) );
                    addChild(energy);
                }
                else {
                    rate += Game::DropRate::EnergyFire;
                    if ( chance < rate ) {
                        log("____ EnergyFire spawned ____");
                        auto energy = EnergyPowerUp::create(BeamType::Fire);
                        energy->setPosition(x, y);
                        energy->setAnchorPoint( Vec2(0.5, 0.5) );
                        addChild(energy);
                    }
                    else {
                        rate += Game::DropRate::ShieldBronze;
                        if ( chance < rate ) {
                            log("____ ShieldBronze spawned ____");
                            auto shield = ShieldPowerUp::create(ShieldLevel::Low);
                            shield->setPosition(x, y);
                            shield->setAnchorPoint( Vec2(0.5, 0.5) );
                            addChild(shield);
                        }
                        else {
                            rate += Game::DropRate::ShieldSilver;
                            if ( chance < rate ) {
                                log("____ ShieldSilver spawned ____");
                                auto shield = ShieldPowerUp::create(ShieldLevel::Medium);
                                shield->setPosition(x, y);
                                shield->setAnchorPoint( Vec2(0.5, 0.5) );
                                addChild(shield);
                            }
                            else {
                                rate += Game::DropRate::ShieldGold;
                                if ( chance < rate ) {
                                    log("____ ShieldGold spawned ____");
                                    auto shield = ShieldPowerUp::create(ShieldLevel::High);
                                    shield->setPosition(x, y);
                                    shield->setAnchorPoint( Vec2(0.5, 0.5) );
                                    addChild(shield);
                                }
                                else {
                                    rate += Game::DropRate::AmmoBronze;
                                    if ( chance < rate ) {
                                        log("____ AmmoBronze spawned ____");
                                        auto power = DamagePowerUp::create(DamageLevel::Single);
                                        power->setPosition(x, y);
                                        power->setAnchorPoint( Vec2(0.5, 0.5) );
                                        addChild(power);
                                    }
                                    else {
                                        rate += Game::DropRate::AmmoSilver;
                                        if ( chance < rate ) {
                                            log("____ AmmoSilver spawned ____");
                                            auto power = DamagePowerUp::create(DamageLevel::Double);
                                            power->setPosition(x, y);
                                            power->setAnchorPoint( Vec2(0.5, 0.5) );
                                            addChild(power);
                                        }
                                        else {
                                            rate += Game::DropRate::AmmoGold;
                                            if ( chance < rate ) {
                                                log("____ AmmoGold spawned ____");
                                                auto power = DamagePowerUp::create(DamageLevel::Triple);
                                                power->setPosition(x, y);
                                                power->setAnchorPoint( Vec2(0.5, 0.5) );
                                                addChild(power);
                                            }
                                            else {
                                                //log("____ Nothing spawned! ____");
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    
    }
    
    
    //player->update(dt);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    /*
    hudLayer->update(dt);
    controlLayer->update(dt);*/
#endif
}
