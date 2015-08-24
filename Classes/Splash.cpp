#include "Splash.hpp"
#include "MenuScene.hpp"
#include "NewGameSinglePlayerScene.hpp"

USING_NS_CC;

Scene* Splash::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = Splash::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool Splash::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    // [[-- ATTENTION: Splash Scene is responsible for setting Game global vars and constants
    
    Game::VISIBLE_SIZE = Director::getInstance()->getVisibleSize();
    Game::VISIBLE_ORIGIN = Director::getInstance()->getVisibleOrigin();
    
    Game::GREATER_DIMENSION = Game::VISIBLE_SIZE.width;
    if (Game::VISIBLE_SIZE.height > Game::VISIBLE_SIZE.width)
        Game::GREATER_DIMENSION = Game::VISIBLE_SIZE.height;
    
    Game::debugDraw = false;
    
    // --]]

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(Splash::menuCloseCallback, this));
    
	closeItem->setPosition(Vec2(Game::VISIBLE_ORIGIN.x + Game::VISIBLE_SIZE.width - closeItem->getContentSize().width/2 ,
                                Game::VISIBLE_ORIGIN.y + closeItem->getContentSize().height/2));

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    // Cria os dois labels do splash
    
    auto backgroundSprite = Sprite::create("background.png");
    
    backgroundSprite->setAnchorPoint(Vec2(0.5, 0.5));
    backgroundSprite->setPosition(Vec2(Game::VISIBLE_SIZE.width/2 + Game::VISIBLE_ORIGIN.x, 
                                       Game::VISIBLE_SIZE.height/2 + Game::VISIBLE_ORIGIN.y));
    //backgroundSprite->runAction(Liquid::create(3, Size(16,12), 4, 20));
    
    this->addChild(backgroundSprite, 0);
    
    auto splashTitle = Sprite::create("splash.png");
    this->addChild(splashTitle, 1);
    
    auto backgroundShadowSprite = Sprite::create("background_shadow.png");
    
    backgroundShadowSprite->setAnchorPoint(Vec2(0.5, 0.5));
    backgroundShadowSprite->setPosition(Vec2(Game::VISIBLE_SIZE.width/2 + Game::VISIBLE_ORIGIN.x, 
                                             Game::VISIBLE_SIZE.height/2 + Game::VISIBLE_ORIGIN.y));
    //backgroundSprite->runAction(Liquid::create(3, Size(16,12), 4, 20));
    
    this->addChild(backgroundShadowSprite, 777);
    
    splashTitle->setPosition(Vec2(Game::VISIBLE_SIZE.width/2 + Game::VISIBLE_ORIGIN.x, 
                                  Game::VISIBLE_SIZE.height/2 + Game::VISIBLE_ORIGIN.y));
    
    splashTitle->setOpacity(0.2);
    
    auto fadeIn = FadeIn::create(1.f);
    auto delay = DelayTime::create(0.0f);
    auto fadeOut = FadeOut::create(1.f);
    auto nextScenecallback = CallFunc::create([](){
        auto scene = new (std::nothrow) NewGameSinglePlayerScene();
        Director::getInstance()->replaceScene(scene);
        scene->release();
    });
    auto seq = Sequence::create(fadeIn, delay, fadeOut, nextScenecallback, nullptr);
    
    splashTitle->runAction(seq);
    //auto spawn = Sequence::create(fadeIn, effect, nullptr);
    
    return true;
}


void Splash::menuCloseCallback(Ref* pSender)
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
