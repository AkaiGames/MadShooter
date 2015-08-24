#include "Ship.hpp"

USING_NS_CC;
using namespace CocosDenshion;

Ship::~Ship()
{
    CC_SAFE_RELEASE(_sprite);
    CC_SAFE_RELEASE(_healthBar);
    CC_SAFE_RELEASE(_lblShipName);
}

void Ship::moveTo( cocos2d::Vec2 pos ) 
{
    if (_enabled ) {
        _sprite->setPositionX(pos.x); _sprite->setPositionY(pos.y);
    }
}

void Ship::updateLabels()
{
    int h_offset = _sprite->getPositionX();
    int v_offset = _sprite->getPositionY() - _sprite->getContentSize().height / 1.7;
    
    _lblShipName->setPositionX( h_offset ); _lblShipName->setPositionY( v_offset );
    
    v_offset -= _lblShipName->getContentSize().height;
    
    _healthBar->setPositionX( h_offset );
    _healthBar->setPositionY( v_offset );
    
    v_offset -= _healthBar->getContentSize().height;
    
    _energyBar->setPositionX( h_offset );
    _energyBar->setPositionY( v_offset );
}

void Ship::setHP(int healthPoints)
{
    if (_enabled) {
        // if there's no need to redraw healthbar just return
        if (healthPoints == this->_healthPoints) return;
        
        if ( healthPoints < 0 ) {
            this->_healthPoints = 0;
        } else if ( healthPoints > Ship::MAX_HEALTH ) {
            this->_healthPoints = Ship::MAX_HEALTH;
        } else {
            this->_healthPoints = healthPoints;
        }
        
        // hp changed, update health bar
        
        // clear the buffer
        _healthBar->clear();
        
        // draw the borders and also clears the health
        _healthBarVerts[0] = Vec2(0,0);
        _healthBarVerts[1] = Vec2(0,10);
        _healthBarVerts[2] = Vec2(102,10);
        _healthBarVerts[3] = Vec2(102,0);
        _healthBar->drawSolidPoly(_healthBarVerts, 4, Color4F(35.0f/255.0f, 28.0f/255.0f, 40.0f/255.0f, .5f));
        // fills the healthbar if there is health
        if ( getHP() > 0 ) {
            Color4F fillColor;
            
            if ( getHP() >= 66 ) {
                fillColor = Color4F(0xf0/255.0f, 0xcc/255.0f, 0x0e/255.0f, 1.0f);
            } else if ( getHP() >= 33 ) {
                fillColor = Color4F(255.0f/255.0f, 126.0f/255.0f, 0.0f/255.0f, 1.0f);
            } else {
                fillColor = Color4F(255.0f/255.0f, 0.0f/255.0f, 0.0f/255.0f, 1.0f);
            }
            
            _healthBarVerts[0] = Vec2(1,1);
            _healthBarVerts[1] = Vec2(1,9);
            _healthBarVerts[2] = Vec2(getHP()+1,9);
            _healthBarVerts[3] = Vec2(getHP()+1,1);
            _healthBar->drawSolidPoly(_healthBarVerts, 4, fillColor);
        }
    }
}

void Ship::setShipName(std::string name)
{
    if (_enabled) {
        _lblShipName->setString(name);
        moveTo( Vec2(getXPos(),getYPos()) ); // just to update ship label
    }
}

ShipInfo Ship::getInfo() const {
    ShipInfo info;
    
    info.ID = getID();
    info.totalScore = _totalScore;
    info.damage = _damage;
    info.defense = _defense;
    info.engineState = _engineState;
    info.angleInDegrees = _angleInDegrees;
    info.healthPoints = _healthPoints;
    info.energyPoints = _energyPoints;
    info.velocity = getVelocity();
    info.beamType = _beamType;
    info.position.x = getXPos();
    info.position.y = getYPos();
    
    return info;
}

void Ship::hitShip(ShipInfo actorInfo, Ship* ship)
{
    // if ship is frozen and is shot by an icy beam.
    bool freezeAgain = false,
         burnAgain = false;
    
    //log("[] Ship(%d)'s hp before: %d", ship->getID(), ship->getHP());
    //log("[] Ship(%d)'s defense: %f", ship->getID(), ship->getDefense());
    //log("[] Attacker's damage: %d", actorInfo.damage);
    
    // [[-- if shield is active, then absorb
         
         log("Before 111");
    
    if ( ship->hasShield() == true ) {
        // absorb
        //log("[] Shield defense is: %f, missile took %f", ship->_shieldDefense, ship->_shieldDefense - actorInfo.damage);
        ship->_shieldDefense -= actorInfo.damage;
        
        // if _shieldDefense is < 0 then the attack passed our shield, then pass the rest to the defense
        if ( ship->_shieldDefense <= 0 ) {
            actorInfo.damage = ship->_shieldDefense*(-1);
            std::ostringstream stream;
            stream << "shield-" << ship->getID();
            auto shieldSprite = (Sprite*) ship->_sprite->getChildByName(stream.str());
            if ( shieldSprite == nullptr ) {
                log("Wops!! No shield, something weird happed in ship %d!", ship->getID());
                return;
            }
            shieldSprite->removeFromParentAndCleanup(true);
            ship->_shieldLevel = ShieldLevel::NoShield;
            ship->_shieldDefense = 0;
            //log(">>>> Shield of shihp %d was destroyed!", ship->getID());
        } else {
            actorInfo.damage = 0;
            // return, the shield will absorb everything
            return;
        }
    }
    
    // --]]
    
    // [[-- DEFENSE
    
    float realDamage = (actorInfo.damage - ship->getDefense());
    
    if ( realDamage > 0 ) {
        //log("======= Ship %d absorbed %f damage.", ship->getID(), ship->getDefense());
        //log("======= Ship %d suffered %f real damage.", ship->getID(), realDamage);
        ship->setHP( ship->getHP() - realDamage );
    } else {
        //log("No damage done on ship %d.", ship->getID());
    }
    
    // --]]

    // ice missile hit ship
    if ( actorInfo.beamType == BeamType::Ice ) {
        // refreshing block of ice, freezing again
        if ( ship->isFrozen() == true ) {
            //log("======> Ship %d removing old block of ice!", ship->getID() );
            ship->_sprite->removeAllChildrenWithCleanup(true);
            ship->enable();
            freezeAgain = true;
        }
        
        // fire melts ice
        if ( ship->isOnFire() == true ) {
            //log("======> Ship %d ice cleaned fire!", ship->getID() );
            ship->_sprite->removeAllChildrenWithCleanup(true);
            ship->enable();
            ship->_onFire = false;
        // ice freezing ship
        } else {
            //log("======> Ship %d frozen!", ship->getID());
            
            ship->disable();
            ship->_frozen = true;
            auto blockOfIce = Sprite::create("block_of_ice.png");
            auto v1 = Vec2(ship->getXPos(), ship->getYPos());
            auto v2 = ship->convertToWorldSpace(v1);
            auto v3 = ship->_sprite->convertToNodeSpace(v2);
            blockOfIce->setPosition( v3 );
            ship->_sprite->addChild(blockOfIce, 777);
            FadeIn* fadeIn = nullptr;
            if ( freezeAgain == false ) {
                fadeIn = FadeIn::create(0.2f);
                blockOfIce->setOpacity(0);
            } else {
                fadeIn = FadeIn::create(0);
            }
            auto delay = DelayTime::create(1000.f);
            auto fadeOut = FadeOut::create(0.2f);
            auto callback1 = CallFunc::create([=](){
                ship->enable();
                ship->_frozen = false;
                ship->_sprite->removeChild(blockOfIce, true);
                //log("======> Ship %d Ice melted!", ship->getID());
            });
            auto seq0 = Sequence::create(fadeIn, delay, fadeOut, callback1, nullptr);
            blockOfIce->runAction(seq0);
        }
    // ok, if it's frozen, refresh the ice block
    }
    // fire missile
    else if ( actorInfo.beamType == BeamType::Fire ) 
    {
        // on fire again
        if ( ship->isOnFire() == true ) {
            //log("======> Ship %d cleaning fire!", ship->getID() );
            ship->_sprite->removeAllChildrenWithCleanup(true);
            ship->enable();
            burnAgain = true;
        }
        
        // fire melts ice
        if ( ship->isFrozen() == true ) {
            //log("======> Ship %d fire melted ice!", ship->getID() );
            ship->_sprite->removeAllChildrenWithCleanup(true);
            ship->enable();
            ship->_frozen = false;
        }
        // put fire on the ship!
        else {
            //log("======> Ship %d on fire!!", ship->getID());
            
            ship->_onFire = true;
            auto blockOfFire = Sprite::create("block_of_fire.png");
            auto v1 = Vec2(ship->getXPos(), ship->getYPos());
            auto v2 = ship->convertToWorldSpace(v1);
            auto v3 = ship->_sprite->convertToNodeSpace(v2);
            blockOfFire->setPosition( v3 );
            ship->_sprite->addChild(blockOfFire, 777);
            // [[-- apply the DOT
            ship->_dot = actorInfo.damage / 40.0; 
            //log("++++ Setting DOT %f on Ship %d", ship->_dot, ship->getID());
            auto dotDelay = DelayTime::create(1.0f);
            auto dotFunction = CallFunc::create([=](){
                //log("_________ Applying DOT %f on Ship %d.", ship->_dot, ship->getID());
                ship->setHP(ship->getHP() - ship->_dot);
                checkIfShipDied(ship);
            });
            auto dotSeq = Sequence::create(dotDelay, dotFunction, nullptr);
            auto dotRepeat = Repeat::create(dotSeq, 50);
            blockOfFire->runAction(dotRepeat);
            // end of dof --]]
            FadeIn* fadeIn = nullptr;
            if ( burnAgain == false ) {
                fadeIn = FadeIn::create(0.2f);
                blockOfFire->setOpacity(0);
            } else {
                fadeIn = FadeIn::create(0);
            }
            auto delay = DelayTime::create(1000.f);
            auto fadeOut = FadeOut::create(0.2f);
            auto callback1 = CallFunc::create([=](){
                ship->enable();
                ship->_onFire = false;
                ship->_sprite->removeChild(blockOfFire, true);
                //log("======> Ship %d Fire over!", ship->getID());
            });
            auto seq0 = Sequence::create(fadeIn, delay, fadeOut, callback1, nullptr);
            blockOfFire->runAction(seq0);
        }
    }
    
    checkIfShipDied(ship); // will automatically remove the ship if it just died.
}

bool Ship::checkIfShipDied(ShipInfo shipInfo) {
    Ship* ship = getParent()->getChildByTag(shipInfo.ID);
    
    if (ship == nullptr)
        return true;
    
    // if hp is zero, schedule the ship for destruction
    if ( ship->getHP() == 0 )
    {
        // disables ship, then fades it out and schedule for destruction after fade out is done
        ship->disable();
        
        // creates the animation actions to simulate a malfunction.
        auto tintTo = TintTo::create(0.5f, 255.0f, 0.0f, 0.0f);
        
        // callback
        auto callback = CallFunc::create([=](){
            // NOTIFY ALL MISSILES OF THIS SHIP that it died and send them the last info about that ship
            
            Vec2 shipPos( ship->getXPos(), ship->getYPos() );
            Node* shipParent = ship->getParent();
            int shipID = ship->getID();
            
            //log("Reference count for this ship: %d", shipReferenceCount );
            
            if ( ship->isBot() == false ) {
                log("It's a player ship! :(");
            }
            
            //log("Removing ship from parent..");
            // remove and clean up
            ship->removeFromParentAndCleanup(true);
            
            // particle emitter to simulate explosion
            // emitter for ExplodingRing particles to simulate explosion.
            auto emitter = ParticleSystemQuad::create("particles/ExplodingRing.plist");
            shipParent->addChild(emitter, 10);
            emitter->setPosition( shipPos );
        });
        auto seq = Sequence::create(tintTo, callback, nullptr);
        ship->_sprite->runAction(seq);
        
        return true;
    }
    
    return false;
}

void Ship::setEnergy(int energyPoints)
{
    if (_enabled) {
        // if there's no need to redraw healthbar just return
        // this line was remove because OF feature #01
        // When the ice/fire effect is over, it need to update
        // if (energyPoints == this->_energyPoints) return;
        
        if ( energyPoints < 0 ) {
            this->_energyPoints = 0;
        } else if ( energyPoints > Ship::MAX_ENERGY ) {
            this->_energyPoints = Ship::MAX_ENERGY;
        } else {
            this->_energyPoints = energyPoints;
        }
        
        // hp changed, update health bar
        
        // clear the buffer
        _energyBar->clear();
        
        // draw the borders and also clears the health
        _energyBarVerts[0] = Vec2(0,0);
        _energyBarVerts[1] = Vec2(0,10);
        _energyBarVerts[2] = Vec2(102,10);
        _energyBarVerts[3] = Vec2(102,0);
        _energyBar->drawSolidPoly(_energyBarVerts, 4, Color4F(35.0f/255.0f, 28.0f/255.0f, 40.0f/255.0f, .5f));
        // fills the healthbar if there is health
        if ( getEnergy() > 0 ) {
            Color4F fillColor;
            
            if ( _beamType == BeamType::Fire ) {
                fillColor.r = 0xe2/255.0f; fillColor.g = 0x24/255.0f; fillColor.b = 0x00/255.0f;
            } else if ( _beamType == BeamType::Normal ) {
                fillColor.r = 0x65/255.0f; fillColor.g = 0xe2/255.0f; fillColor.b = 0x00/255.0f;
            } else if ( _beamType == BeamType::Ice ) {
                fillColor.r = 0x00/255.0f; fillColor.g = 0xa5/255.0f; fillColor.b = 0xe2/255.0f;
            }
            
            fillColor.a = getEnergy() / ((double)Ship::MAX_ENERGY);
            
            _energyBarVerts[0] = Vec2(1,1);
            _energyBarVerts[1] = Vec2(1,9);
            _energyBarVerts[2] = Vec2(getEnergy()+1,9);
            _energyBarVerts[3] = Vec2(getEnergy()+1,1);
            _energyBar->drawSolidPoly(_energyBarVerts, 4, fillColor);
        }
    }
}

void Ship::fire()
{
    if (_enabled) {
        // engine's not cooled yet and has enough energy
        if (_gunHeat < 0.2f || getEnergy() == 0) 
            return;

        auto b = Missile::create(_beamType);
        b->_sourceShipID = getID();
        
        Vec2 bulletPos = Vec2( this->getXPos(), this->getYPos() );
        bulletPos.x += 63 * cos( -CC_DEGREES_TO_RADIANS(_angleInDegrees) );
        bulletPos.y += 63 * sin( -CC_DEGREES_TO_RADIANS(_angleInDegrees) );
        
        b->setPosition( bulletPos );
        b->setRotation( _angleInDegrees );
        //b->setTarget(this); // TODO: Feature not implemented yet
        b->getPhysicsBody()->applyImpulse(Vec2 ( 800 * cos( -CC_DEGREES_TO_RADIANS(_angleInDegrees) ), 
               800 * sin( -CC_DEGREES_TO_RADIANS(_angleInDegrees) ) ));
        getParent()->addChild(b);
        
        // if this ship dies, we'll notify all bullets in this list.
        //_bullets.push_back(b); // this ship fired this bullet, so store it.
        
        /*if (_playingLaser == false) {
            _playingLaser = true;
            //SimpleAudioEngine::getInstance()->playEffect("audio/sfx_laser1.ogg");
            auto delay = DelayTime::create(0.2f);
            auto func = CallFunc::create([=](){
                Ship::_playingLaser = false;
            });
            auto seq = Sequence::create(delay, func, nullptr);
            _sprite->runAction(seq);
        }*/
        
        _gunHeat = 0;
        setEnergy( getEnergy() - 1 );
    }
}

void Ship::setVelocity(const Vec2& vel)
{
    if (_enabled) {
        _sprite->getPhysicsBody()->setVelocity( vel );
    }
}

void Ship::enable()
{
    _enabled = true;
}

void Ship::disable() 
{
    turnEngine(false);
    stopTurning();
    _enabled = false;
}

void Ship::turnEngine(bool state)
{
    if (_enabled) {
        _engineState = state;
    }
}

void Ship::force(Vec2 vec) 
{
    if (_enabled) {
        _sprite->getPhysicsBody()->resetForces();
        _sprite->getPhysicsBody()->applyImpulse( vec );
    }
}

void Ship::toggleEngine()
{
    if (_enabled) {
        if (_engineState == true)
            turnEngine( false );
        else
            turnEngine( true );
    }
}

void Ship::turnShip(float angle)
{
    if ( _enabled == true && _isTurning == false ) 
    {
        _isTurning = true;
        _targetTurningAngle = angle;
        _totalAngleTurned = 0;
    }
}

void Ship::registerPlayer(std::string playerName)
{
    if ( _isBot == true ) { 
        _isBot = false;
        
        setShipName(playerName);
        
        std::stringstream stream;
        stream << "ships/Player/" << (cocos2d::random() % 12) + 1 << ".png";
        _sprite->setTexture(stream.str().c_str());
        
        auto physicBody = PhysicsBody::createCircle( getRadius() , PhysicsMaterial(0, 0, 0) );
        physicBody->setCategoryBitmask(Type::SHIP);
        physicBody->setContactTestBitmask(Type::SHIP | Type::MISSILE | Type::BORDER);
        
        _sprite->setPhysicsBody( physicBody );
    } else {
        log("Object(%d) already registered as a player!", getID());
    }
}

bool Ship::init()
{
    if ( !Entity::init() ){
        return false;
    }
    
    enable();
    
    // [[-- Main sprite
    
    // gets a random ship
    std::stringstream stream;
    stream << "ships/Enemies/" << (cocos2d::random() % 20) + 1 << ".png";
    _sprite = Sprite::create(stream.str().c_str());
    _sprite->setOpacity(0);
    auto fadeIn = FadeIn::create(1.0f);
    _sprite->runAction(fadeIn);
    
    // [[-- sets the physic body
    auto physicBody = PhysicsBody::createCircle( getRadius() , PhysicsMaterial(0, 0, 0) );
    physicBody->setCategoryBitmask(Type::SHIP);
    //physicBody->setCollisionBitmask(ENTITY_SHIP | ENTITY_MISSILE | ENTITY_BORDER);
    physicBody->setContactTestBitmask(Type::SHIP | Type::MISSILE | Type::BORDER);
    
    _sprite->setTag(getID());
    
    //physicBody->setTag();
    _sprite->setPhysicsBody( physicBody );
    // --]]
    
    _sprite->setFlippedY(true);
    _sprite->setAnchorPoint(Vec2(0.5, 0.5));
    addChild(_sprite, 1);
    
    _sprite->retain();
    
    // --]]
    
    // [[-- Player Name Label
    
    _lblShipName = Label::createWithTTF("UNKNOWN_SHIP","fonts/kenvector_future_thin.ttf", 10);
    _lblShipName->retain();
    addChild(_lblShipName, 2);
    
    // --]]
    
    // [[-- Health Bar
    
    _healthBar = DrawNode::create();
    _healthBar->setAnchorPoint(Vec2(0.5,0.5));
    _healthBar->retain();
    _healthBar->setContentSize(Size(102, 10));
    
    addChild(_healthBar);
    
    // --]]
    
    // [[-- Energy Bar
    
    _energyBar = DrawNode::create();
    _energyBar->setAnchorPoint(Vec2(0.5,0.5));
    _energyBar->retain();
    _energyBar->setContentSize(Size(102, 10));
    
    addChild(_energyBar);
    
    // --]]
    
    // move ship to desired pos
    moveTo( Vec2(Game::VISIBLE_SIZE.width/2 + Game::VISIBLE_ORIGIN.x, 
                 Game::VISIBLE_SIZE.height/2 + Game::VISIBLE_ORIGIN.y) );
    
    // [[-- Set ship's attributes
    _damage = 25;
    _defense = 5;
    _shieldDefense = 0;
    _shieldLevel = ShieldLevel::NoShield;
    _beamType = BeamType::Normal; // starts with normal beam, needs to be set before drawing the bars
    _energyPoints = 0; // needs to be initialized!
    _healthPoints = 0; // needs to be initialized!
    setHP(Ship::MAX_HEALTH);
    setEnergy(Ship::MAX_ENERGY);
    updateLabels(); // update
    setVelocity( Vec2(0,0) );
    _engineState = false;
    // --]]
    
    // Position has not changed, and angle is 0
    _targetPositionChanged = false;
    _isTurning = false;
    _sprite->setRotation(0);
    _angleInDegrees = 0;
    
    _type = Type::SHIP;
    _isBot = true;
    
    _gunHeat = 0;
    
    _playingLaser = false;
    
    _frozen = false;
    _onFire = false;
    _dot = 0;
    
    _showRayCast = true;
    _rayNode = DrawNode::create();
    addChild(_rayNode);
    
    return true;
}

bool Ship::isFrozen() const
{
    return _frozen;
}

bool Ship::isOnFire() const
{
    return _onFire;
}

bool Ship::hasShield() const
{
    return _shieldLevel != ShieldLevel::NoShield;
}

bool Ship::isBot() const
{
    return _isBot;
}

int Ship::getXPos() const
{
    return _sprite->getPositionX();
}

int Ship::getYPos() const
{
    return _sprite->getPositionY();
}

std::string Ship::getShipName() const
{
    return _lblShipName->getString();
}

int Ship::getHP() const
{
    return _healthPoints;
}

int Ship::getEnergy() const
{
    return _energyPoints;
}

int Ship::getDamage() const
{
    return _damage;
}

float Ship::getDefense() const
{
    return _defense;
}

float Ship::getHeading() const
{
    return (_angleInDegrees < 0 ) ? _angleInDegrees * -1 : 360 - _angleInDegrees;
}

float Ship::getRadius() const
{
    return _sprite->getContentSize().width / 2;
}

Vec2 Ship::getVelocity() const
{
    return _sprite->getPhysicsBody()->getVelocity();
}

bool Ship::getEngineState() const
{
    return _engineState;
}

bool Ship::isEnabled() const
{
    return _enabled;
}

void Ship::stopTurning()
{
    _isTurning = false;
}

bool Ship::isTurning() const
{
    return _isTurning;
}

bool Ship::rayCastCallback(PhysicsWorld& world, const PhysicsRayCastInfo& info, void* data)
{
    *((Vec2*)data) = info.contact;
    
    if ( info.shape->getCategoryBitmask() == Entity::SHIP ) {
        // checks if we already marked this ship as seen by consulting the map of shapes in line of sight
        LineOfSight::iterator iter = _inLineOfSight.find( info.shape->getBody()->getNode()->getTag() );
        // if we did not spot it yet, add it to the list of objects in line of sight
        if ( iter == _inLineOfSight.end() ) {
            _inLineOfSight.insert( std::make_pair( info.shape->getBody()->getNode()->getTag(), true) );
            //log("Spotted node %d", info.shape->getBody()->getNode()->getTag() );
            this->onShipEnteredLineOfSight( ((Ship*)(info.shape->getBody()->getNode()->getParent()))->getInfo() );
        } else {
            iter->second = true;
        }
    }
    
    return false;
}

void Ship::update(float dt)
{
    Entity::update(dt);
    
    _gunHeat += dt;
    
    // if ship is disabled, it will not move!
    if ( _enabled == true ) {
    
        if ( _isTurning == true )
        {
            if ( _totalAngleTurned == _targetTurningAngle )
            {
                _isTurning = false;
                _angleInDegrees = _sprite->getRotation();
            }
            else if ( _targetTurningAngle < 0 )
            {
                _totalAngleTurned -= 1;
                _angleInDegrees -= 1;
                _sprite->setRotation(_angleInDegrees - 1);
            }
            else if ( _targetTurningAngle > 0 )
            {
                _totalAngleTurned += 1;
                _angleInDegrees += 1;
                _sprite->setRotation(_angleInDegrees + 1);
            }
            
            if ( _angleInDegrees > 360 )
                _angleInDegrees -= 360;
            else if ( _angleInDegrees < -360 )
                _angleInDegrees += 360;
            
            //log("[Ship%d] Heading(%f)", getID(), getHeading()); 
            
            _aimVector.x = 2 * cos( -CC_DEGREES_TO_RADIANS(_angleInDegrees) );
            _aimVector.y = 2 * sin( -CC_DEGREES_TO_RADIANS(_angleInDegrees) );
        }
        
        // if engine is on, accelerate
        if (getEngineState() == true ) 
        {
            force( Vec2( (int)_aimVector.x % 5, (int)_aimVector.y % 5 ) ); // apply a force - acceleration.
        }
    }
    
    // [[-- Ray Casting to mock 'line of sight'
        
    // for all elements in map, set seen to false
    for ( LineOfSight::iterator iter = _inLineOfSight.begin(); iter != _inLineOfSight.end(); iter++ ) {
        iter->second = false;
    }
        
    _rayNode->clear();
    
    for (float alfa = _angleInDegrees - 10; alfa < _angleInDegrees + 10; alfa++) {        
        
        Vec2 p1 = Vec2(getXPos() + (getRadius() + 7) * cos( -CC_DEGREES_TO_RADIANS(alfa)), 
                        getYPos() + (getRadius() + 7) * sin( -CC_DEGREES_TO_RADIANS(alfa)) );
        Vec2 distance( (Game::GREATER_DIMENSION + (2 * getRadius())) * cos( -CC_DEGREES_TO_RADIANS(alfa)) ,
                        (Game::GREATER_DIMENSION + (2 * getRadius())) * sin( -CC_DEGREES_TO_RADIANS(alfa)) ) ;
        Vec2 p2 = p1 + distance;
        Vec2 p3 = p2;
        
        auto func = CC_CALLBACK_3(Ship::rayCastCallback, this);
        getScene()->getPhysicsWorld()->rayCast(func, p1, p2, &p3);
        
        if ( Game::debugDraw == true ) {
            _rayNode->drawSegment(p1, p3, 1, Color4F(1.f, 0.f, 0.f, 1.f));
            if (p2 != p3)
            {
                _rayNode->drawDot(p3, 2, Color4F(1.0f, 1.0f, 1.0f, 1.0f));
            };
        }
    }
    
    // check if any shape ran out of sight and if yes, remove it from the line of sight map
    for ( LineOfSight::iterator iter = _inLineOfSight.begin(); iter != _inLineOfSight.end(); iter++ ) {
        if ( iter->second == false ) {
            if ( Entity::isAlive( iter->first ) == false ) {
                //log("Node %d died in front of me.", iter->first );
                //this->onShipDied( iter->first );
            } else {
                //log("Node %d ran out of sight.", iter->first );
                //this->onShipOutOfLineOfSight( ((Ship*)(Entity::getEntity( iter->first )))->getInfo() );
            }
            
            _inLineOfSight.erase( iter );
        }
    }
    
    // [[-- End of ray casting..
    
    updateLabels();
}


