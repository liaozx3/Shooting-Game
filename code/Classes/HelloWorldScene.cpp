#include "HelloWorldScene.h"
#include "GameScene.h"
#pragma execution_character_set("utf-8")
USING_NS_CC;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }

    visibleSize = Director::getInstance()->getVisibleSize();
    origin = Director::getInstance()->getVisibleOrigin();

	addKeyboardListener();
	schedule(schedule_selector(HelloWorld::update), 0.04f, kRepeatForever, 0);

	TMXTiledMap* tmx = TMXTiledMap::create("a.tmx");
	tmx->setPosition(visibleSize.width / 2, visibleSize.height / 2);
	tmx->setAnchorPoint(Vec2(0.5, 0.5));
	tmx->setScale(Director::getInstance()->getContentScaleFactor());
	this->addChild(tmx, 0);

	startItem = MenuItemImage::create("start2.png",
		"start3.png",
		CC_CALLBACK_1(HelloWorld::menuStartCallback, this));
	startItem->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y - 95));
	startItem->setVisible(false);
	auto menu = Menu::create(startItem, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);

	bg = Sprite::create("title1.jpg");
	bg->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y + 50));
	bg->setVisible(false);
	this->addChild(bg, 1);
	bg1 = Sprite::create("title2.jpg");
	bg1->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y + 50));
	bg1->setVisible(false);
	this->addChild(bg1, 1);
	bg2 = Sprite::create("title3.jpg");
	bg2->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y + 50));
	bg2->setVisible(false);
	this->addChild(bg2, 1);
	bg3 = Sprite::create("title4.jpg");
	bg3->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y + 50));
	bg3->setVisible(false);
	this->addChild(bg3, 1);

	isMove = false;

	//创建一张贴图
	auto texture = Director::getInstance()->getTextureCache()->addImage("player3.png");
	//从贴图中以像素单位切割，创建关键帧
	auto frame0 = SpriteFrame::createWithTexture(texture, CC_RECT_PIXELS_TO_POINTS(Rect(0, 0, 120, 120)));
	//使用第一帧创建精灵
	player = Sprite::createWithSpriteFrame(frame0);
	player->setPosition(Vec2(origin.x + visibleSize.width / 4 - 15,
		origin.y + 2.8 * visibleSize.height / 4));
	addChild(player, 2);
    
	//动画帧
	runSouth.reserve(4);
	runEast.reserve(4);
	runNorth.reserve(4);
	runWest.reserve(4);
	SpriteFrame* frame;
	for (int j = 0; j < 4; j++) {
		for (int i = 0; i < 4; i++) {
			frame = SpriteFrame::createWithTexture(texture, CC_RECT_PIXELS_TO_POINTS(Rect(120 * i, 120 * j, 120, 120)));
			switch (j) {
			case 0:
				runSouth.pushBack(frame);
				break;
			case 1:
				runEast.pushBack(frame);
				break;
			case 2:
				runNorth.pushBack(frame);
				break;
			case 3:
				runWest.pushBack(frame);
				break;
			}
		}
	}
    return true;
}

void HelloWorld::Aaction(Ref * pSender) {
	if (player->numberOfRunningActions() != 0) return;
	auto pos = player->getPosition();
	if (pos.x - 20 > origin.x) {
		CCFiniteTimeAction *actionMove = CCMoveTo::create(0.5, Vec2(pos.x - 30, pos.y));
		player->runAction(actionMove);
	}
	auto animation = Animation::createWithSpriteFrames(runWest, 0.12f);
	auto animate = Animate::create(animation);
	player->runAction(animate);
}

void HelloWorld::Daction(Ref * pSender) {
	if (player->numberOfRunningActions() != 0) return;
	auto pos = player->getPosition();
	if (pos.x + 20 < origin.x + visibleSize.width) {
		CCFiniteTimeAction *actionMove = CCMoveTo::create(0.5, Vec2(pos.x + 30, pos.y));
		player->runAction(actionMove);
	}
	auto animation = Animation::createWithSpriteFrames(runEast, 0.12f);
	auto animate = Animate::create(animation);
	player->runAction(animate);
}

void HelloWorld::Waction(Ref * pSender) {
	if (player->numberOfRunningActions() != 0) return;
	auto pos = player->getPosition();
	if (pos.y + 20 < origin.y + visibleSize.height) {
		CCFiniteTimeAction *actionMove = CCMoveTo::create(0.5, Vec2(pos.x, pos.y + 30));
		player->runAction(actionMove);
	}
	auto animation = Animation::createWithSpriteFrames(runNorth, 0.12f);
	auto animate = Animate::create(animation);
	player->runAction(animate);
}

void HelloWorld::Saction(Ref * pSender) {
	if (player->numberOfRunningActions() != 0) return;
	auto pos = player->getPosition();
	if (pos.y - 20 > origin.y) {
		CCFiniteTimeAction *actionMove = CCMoveTo::create(0.5, Vec2(pos.x, pos.y - 30));
		player->runAction(actionMove);
	}
	auto animation = Animation::createWithSpriteFrames(runSouth, 0.12f);
	auto animate = Animate::create(animation);
	player->runAction(animate);
}

void HelloWorld::addKeyboardListener() {
	auto keyboardListener = EventListenerKeyboard::create();
	keyboardListener->onKeyPressed = CC_CALLBACK_2(HelloWorld::onKeyPressed, this);
	keyboardListener->onKeyReleased = CC_CALLBACK_2(HelloWorld::onKeyReleased, this);
	_eventDispatcher->addEventListenerWithFixedPriority(keyboardListener, 1);
}

void HelloWorld::onKeyPressed(EventKeyboard::KeyCode code, Event* event) {
	switch (code) {
	case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
	case EventKeyboard::KeyCode::KEY_CAPITAL_A:
	case EventKeyboard::KeyCode::KEY_A:
		movekey = 'A';
		isMove = true;
		break;
	case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
	case EventKeyboard::KeyCode::KEY_CAPITAL_D:
	case EventKeyboard::KeyCode::KEY_D:
		movekey = 'D';
		isMove = true;
		break;
	case EventKeyboard::KeyCode::KEY_UP_ARROW:
	case EventKeyboard::KeyCode::KEY_CAPITAL_W:
	case EventKeyboard::KeyCode::KEY_W:
		movekey = 'W';
		isMove = true;
		break;
	case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
	case EventKeyboard::KeyCode::KEY_CAPITAL_S:
	case EventKeyboard::KeyCode::KEY_S:
		movekey = 'S';
		isMove = true;
		break;
	}
}

void HelloWorld::onKeyReleased(EventKeyboard::KeyCode code, Event* event) {
	switch (code) {
	case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
	case EventKeyboard::KeyCode::KEY_A:
	case EventKeyboard::KeyCode::KEY_CAPITAL_A:
	case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
	case EventKeyboard::KeyCode::KEY_D:
	case EventKeyboard::KeyCode::KEY_CAPITAL_D:
	case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
	case EventKeyboard::KeyCode::KEY_CAPITAL_S:
	case EventKeyboard::KeyCode::KEY_S:
	case EventKeyboard::KeyCode::KEY_UP_ARROW:
	case EventKeyboard::KeyCode::KEY_CAPITAL_W:
	case EventKeyboard::KeyCode::KEY_W:
		isMove = false;
		break;
	}
}

void HelloWorld::update(float f) {
	if (isMove)
		this->movePlane(movekey);
	if (player->getPosition() == Vec2(origin.x + visibleSize.width / 4 - 15, origin.y + 2.8 * visibleSize.height / 4 + 30)) {
		bg->setVisible(true);
		startItem->setVisible(true);
	}
	else if (player->getPosition() == Vec2(origin.x + visibleSize.width / 4 - 15 + 30 * 17, origin.y + 2.8 * visibleSize.height / 4 + 30)) {
		bg1->setVisible(true);
		startItem->setVisible(true);
	}
	else if (player->getPosition() == Vec2(origin.x + visibleSize.width / 4 - 15, origin.y + 2.8 * visibleSize.height / 4 - 30 * 13)) {
		bg2->setVisible(true);
		startItem->setVisible(true);
	}
	else if (player->getPosition() == Vec2(origin.x + visibleSize.width / 4 - 15 + 30 * 17, origin.y + 2.8 * visibleSize.height / 4 - 30 * 13)) {
		bg3->setVisible(true);
		startItem->setVisible(true);
	}
	else {
		bg->setVisible(false);
		bg1->setVisible(false);
		bg2->setVisible(false);
		bg3->setVisible(false);
		startItem->setVisible(false);
	}
}

void HelloWorld::menuStartCallback(Ref* pSender) {
	auto scene = GameSence::createScene();
	auto slidein = TransitionSlideInL::create(1.0f, scene);
	//_eventDispatcher->removeAllEventListeners();
	Director::getInstance()->replaceScene(slidein);
}

void HelloWorld::movePlane(char c)
{
	Ref * pSender = nullptr;
	switch (c)
	{
	case 'A':
		Aaction(pSender);
		break;
	case 'D':
		Daction(pSender);
		break;
	case 'W':
		Waction(pSender);
		break;
	case 'S':
		Saction(pSender);
		break;
	}
}
