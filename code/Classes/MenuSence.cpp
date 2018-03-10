#include "MenuSence.h"
#include "HelloWorldScene.h"
#include "GameScene.h"
USING_NS_CC;

Scene* MenuSence::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();

    // 'layer' is an autorelease object
    auto layer = MenuSence::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool MenuSence::init()
{

    if ( !Layer::init() )
    {
        return false;
    }

    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto bg = Sprite::create("back.jpg");
	bg->setScale(2.5);
	bg->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
	this->addChild(bg, 0);

	auto label = LabelTTF::create("WATCH OUT",//要显示字符串
		"Marker Felt",//字体
		130,//字号
		Size(600, 600),//显示的宽和高
		TextHAlignment::CENTER,//显示的位置，定位
		TextVAlignment::TOP);
	label->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y - 100));
	this->addChild(label, 0);

	auto startItem = MenuItemImage::create("start.jpg",
											"start.jpg",
											CC_CALLBACK_1(MenuSence::menuStartCallback, this));
	startItem->setPosition(Vec2(visibleSize.width/2 + origin.x, origin.y + 150));

	auto menu = Menu::create(startItem, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 2);

    return true;
}

void MenuSence::menuStartCallback(Ref* pSender)
{
	auto scene = HelloWorld::createScene();
	auto slidein = TransitionCrossFade::create(1.0f, scene);
	Director::getInstance()->replaceScene(slidein);
}


