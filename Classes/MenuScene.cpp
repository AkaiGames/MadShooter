#include "MenuScene.hpp"
#include "NewGameSinglePlayerScene.hpp"

USING_NS_CC;

MenuScene::MenuScene()
{
    Scene::init();
}

void newGame_callback(Ref *sender )
{
    /*// create a scene. it's an autorelease object
    auto scene = Splash::createScene();
    // run
    Director::getInstance()->runWithScene(scene);*/
    auto scene = new (std::nothrow) NewGameSinglePlayerScene();
    Director::getInstance()->replaceScene(scene);
    scene->release();
}

void continue_callback(Ref *sender )
{
    log("Continue selected");
}

void multiplayer_callback(Ref *sender )
{
    log("Multiplayer selected");
}

void credits_callback(Ref *sender )
{
    log("Credits selected");
}

void quit_callback(Ref *sender )
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
    MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
    return;
#endif

    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

void MenuScene::onEnter()
{
    Scene::onEnter();
    
    int v_spacing = 0;
    
    auto backgroundSprite = Sprite::create("background.png");
    
    backgroundSprite->setAnchorPoint(Vec2(0.5, 0.5));
    backgroundSprite->setPosition(Vec2(Game::VISIBLE_SIZE.width/2 + Game::VISIBLE_ORIGIN.x, 
                                       Game::VISIBLE_SIZE.height/2 + Game::VISIBLE_ORIGIN.y));
    
    addChild(backgroundSprite, 0);
    
    auto lblTitle = Label::createWithTTF("MadShooter", "fonts/kenvector_future_thin.ttf", 50);
    v_spacing = lblTitle->getLineHeight();
    
    lblTitle->setPosition(Vec2(Game::VISIBLE_SIZE.width/2 + Game::VISIBLE_ORIGIN.x, 
                               Game::VISIBLE_SIZE.height - v_spacing ));
    lblTitle->setAnchorPoint(Vec2(0.5,0.5));
    addChild(lblTitle, 1);

    // fontconfig for the menu items
    TTFConfig ttfConfig("fonts/kenvector_future_thin.ttf", 25);
    
    auto lblNewGame = Label::createWithTTF(ttfConfig,"new game");
    auto lblContinue = Label::createWithTTF(ttfConfig,"continue");
    auto lblMultiplayer = Label::createWithTTF(ttfConfig,"multiplayer");
    auto lblCredits = Label::createWithTTF(ttfConfig,"credits");
    auto lblQuit = Label::createWithTTF(ttfConfig,"quit");

    auto itemNewGame = MenuItemLabel::create(lblNewGame, newGame_callback );
    auto itemContinue = MenuItemLabel::create(lblContinue, continue_callback );
    auto itemMultiplayer = MenuItemLabel::create(lblMultiplayer, multiplayer_callback );
    auto itemCredits = MenuItemLabel::create(lblCredits, credits_callback );
    auto itemQuit = MenuItemLabel::create(lblQuit, quit_callback );
    auto menu = Menu::create(itemNewGame, itemContinue, itemMultiplayer, itemCredits, itemQuit, nullptr);

    menu->setPosition( Vec2::ZERO );
    menu->setAnchorPoint( Vec2(0.5, 0.5) );

    v_spacing -= lblTitle->getLineHeight() - Game::VISIBLE_SIZE.height / 2.5 -  10;
    
    itemNewGame->setPosition( Vec2( Game::VISIBLE_SIZE.width / 2 - lblNewGame->getWidth(), 
                                 Game::VISIBLE_SIZE.height - v_spacing ) );
    
    v_spacing += lblNewGame->getLineHeight() + 10;
    
    itemContinue->setPosition( Vec2( Game::VISIBLE_SIZE.width / 2 - lblContinue->getWidth(), 
                                 Game::VISIBLE_SIZE.height - v_spacing ) );
    
    v_spacing += lblContinue->getLineHeight() + 10;
    
    itemMultiplayer->setPosition( Vec2( Game::VISIBLE_SIZE.width / 2 - lblMultiplayer->getWidth(), 
                                 Game::VISIBLE_SIZE.height - v_spacing ) );
    
    v_spacing += lblContinue->getLineHeight() + 10;
    
    itemCredits->setPosition( Vec2( Game::VISIBLE_SIZE.width / 2 - lblCredits->getWidth(), 
                                 Game::VISIBLE_SIZE.height - v_spacing ) );
    
    v_spacing += lblCredits->getLineHeight() + 10;
    
    itemQuit->setPosition( Vec2( Game::VISIBLE_SIZE.width / 2 - lblQuit->getWidth(), 
                                 Game::VISIBLE_SIZE.height - v_spacing ) );

    addChild(menu, 2);
    
    auto backgroundShadowSprite = Sprite::create("background_shadow.png");
    
    backgroundShadowSprite->setAnchorPoint(Vec2(0.5, 0.5));
    backgroundShadowSprite->setPosition(Vec2(Game::VISIBLE_SIZE.width/2 + Game::VISIBLE_ORIGIN.x, 
                                             Game::VISIBLE_SIZE.height/2 + Game::VISIBLE_ORIGIN.y));
    //backgroundSprite->runAction(Liquid::create(3, Size(16,12), 4, 20));
    
    this->addChild(backgroundShadowSprite, 777);
}