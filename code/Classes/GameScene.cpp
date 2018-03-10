#include "GameScene.h"
#include "SimpleAudioEngine.h"
#include "HelloWorldScene.h"
#include <algorithm>

using namespace std;
using namespace CocosDenshion;
USING_NS_CC;

Scene* GameSence::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = GameSence::create();
	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool GameSence::init()
{
	if (!Layer::init())
	{
		return false;
	}
	//_eventDispatcher->removeAllEventListeners();

	visibleSize = Director::getInstance()->getVisibleSize();
	origin = Director::getInstance()->getVisibleOrigin();

	addKeyboardListener();
	addCustomListener();

	preloadMusic();     // 预加载音乐
	playBgm();          // 播放背景音乐
	
	schedule(schedule_selector(GameSence::update), 0.04f, kRepeatForever, 0);

	TMXTiledMap* tmx = TMXTiledMap::create("tilemap.tmx");
	tmx->setPosition(visibleSize.width / 2, visibleSize.height / 2);
	tmx->setAnchorPoint(Vec2(0.5, 0.5));
	tmx->setScale(Director::getInstance()->getContentScaleFactor());
	this->addChild(tmx, 0);

	isMove = false;
	isShoot = false;
	life = 3;
	score = 0;
	enemyNum = 5;
	count = 0;

	Life = Label::createWithTTF("Life: 3", "fonts/arial.TTF", 30);
	Life->setColor(Color3B(255, 255, 255));
	Life->setPosition(50, 60);
	this->addChild(Life, 3);

	Score = Label::createWithTTF("    Score: 0", "fonts/arial.TTF", 30);
	Score->setColor(Color3B(255, 255, 255));
	Score->setPosition(150, 60);
	this->addChild(Score, 3);

	for (int i = 0; i < 5; i++) {
		enemy[i] = Sprite::create("enemy.png");
		float x = random(origin.x+50, visibleSize.width-50);
		float y = random(origin.y+50, visibleSize.height-50);
		enemy[i]->setPosition(Vec2(x, y));
		this->addChild(enemy[i], 1);
		surviveEnemy[i] = true;
	}


	//创建一张贴图
	auto texture = Director::getInstance()->getTextureCache()->addImage("player1.png");
	//从贴图中以像素单位切割，创建关键帧
	auto frame0 = SpriteFrame::createWithTexture(texture, CC_RECT_PIXELS_TO_POINTS(Rect(0, 0, 50, 100)));
	//使用第一帧创建精灵
	player = Sprite::createWithSpriteFrame(frame0);
	player->setPosition(origin.x + 250, origin.y + 250);
	addChild(player, 2);

	//动画帧
	runSouth.reserve(4);
	runEast.reserve(4);
	runNorth.reserve(4);
	runWest.reserve(4);
	SpriteFrame* frame;
	for (int j = 0; j < 4; j++) {
		for (int i = 0; i < 4; i++) {
			frame = SpriteFrame::createWithTexture(texture, CC_RECT_PIXELS_TO_POINTS(Rect(50 * i, 100 * j, 50, 100)));
			switch (j) {
			case 0:
				runSouth.pushBack(frame);
				break;
			case 1:
				runWest.pushBack(frame);
				break;
			case 2:
				runEast.pushBack(frame);
				break;
			case 3:
				runNorth.pushBack(frame);
				break;
			}
		}
	}

	return true;
}

void GameSence::preloadMusic() {
	auto audio = SimpleAudioEngine::getInstance();

	audio->preloadEffect("music/hurt.wav");
	audio->preloadBackgroundMusic("music/music.mp3");
	audio->preloadEffect("music/shoot.wav");
	audio->preloadEffect("music/explore.wav");
}

void GameSence::playBgm() {
	auto audio = SimpleAudioEngine::getInstance();
	audio->playBackgroundMusic("music/music.mp3", true);
}

// 添加自定义监听器
void GameSence::addCustomListener() {
	auto meetListener = EventListenerCustom::create("meet", CC_CALLBACK_1(GameSence::meet, this));
	_eventDispatcher->addEventListenerWithFixedPriority(meetListener, 1);
}

void GameSence::Aaction(Ref * pSender) {
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

void GameSence::Daction(Ref * pSender) {
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

void GameSence::Waction(Ref * pSender) {
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

void GameSence::Saction(Ref * pSender) {
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


void GameSence::addKeyboardListener() {
	_eventDispatcher->removeAllEventListeners();
	auto keyboardListener = EventListenerKeyboard::create();
	keyboardListener->onKeyPressed = CC_CALLBACK_2(GameSence::onKeyPressed, this);
	keyboardListener->onKeyReleased = CC_CALLBACK_2(GameSence::onKeyReleased, this);
	_eventDispatcher->addEventListenerWithFixedPriority(keyboardListener, 1);
}

void GameSence::onKeyPressed(EventKeyboard::KeyCode code, Event* event) {
	switch (code) {
	case EventKeyboard::KeyCode::KEY_SPACE:
		isShoot = true;
		break;
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

void GameSence::onKeyReleased(EventKeyboard::KeyCode code, Event* event) {
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
	case EventKeyboard::KeyCode::KEY_SPACE:
		isShoot = false;
		isMove = false;
		break;
	}
}

void GameSence::update(float f) {
	// 分发自定义事件
	EventCustom e("meet");
	_eventDispatcher->dispatchEvent(&e);

	count++;
	Vec2 direction[8];
	direction[0] = Vec2(visibleSize.width, 0);
	direction[1] = Vec2(visibleSize.width, visibleSize.height);
	direction[2] = Vec2(0, -visibleSize.height);
	direction[3] = Vec2(-visibleSize.width, -visibleSize.height);
	direction[4] = Vec2(-visibleSize.width, 0);
	direction[5] = Vec2(-visibleSize.width, visibleSize.height);
	direction[6] = Vec2(0, visibleSize.height);
	direction[7] = Vec2(visibleSize.width, visibleSize.height);
	if (count % 50 == 0 && enemyNum != 0) {
		count = 0;
		for (int i = 0; i < 5; i++) {
			if (surviveEnemy[i]) {
				Vec2 pos = enemy[i]->getPosition();
				//Sprite* bullet[8];
				for (int j = 0; j < 8; j++) {
					Sprite* bullet;
					bullet = Sprite::create("bullet2.png");
					bullet->setScale(2);
					bullet->setPosition(pos);
					bullets1.push_back(bullet);
					this->addChild(bullet, 1);
					Sprite* temp = bullet;
					bullet->runAction(
						Sequence::create(
							MoveBy::create(4.0f, direction[j]),
							CallFunc::create([=] {
							bullets1.remove(bullet);
						temp->removeFromParentAndCleanup(true);
					}), nullptr)
					);
				}
			}
		}
	}
	if (isMove) {
		this->movePlane(movekey);
		return;
	}
	if (isShoot) {
		Vec2 pos = player->getPosition();
		Sprite* bullet = Sprite::create("bullet0.png");
		bullet->setScale(2);
		bullet->setPosition(pos);
		bullets.push_back(bullet);
		this->addChild(bullet, 1);
		Sprite* temp = bullet;
		
		SimpleAudioEngine::getInstance()->playEffect("music/shoot.wav", false);

		switch (movekey)
		{
		case 'A':
			isShoot = false;
			bullet->setRotation(180);
			bullet->runAction(
				Sequence::create(
					MoveBy::create(1.0f, Vec2(-visibleSize.width, 0)),
						CallFunc::create([=] {
						bullets.remove(bullet);
						temp->removeFromParentAndCleanup(true);
						
						}), nullptr)
			);
			break;
		case 'D':
			isShoot = false;
			bullet->runAction(
				Sequence::create(
					MoveBy::create(1.0f, Vec2(visibleSize.width, 0)),
					CallFunc::create([=] {
				bullets.remove(bullet);
				temp->removeFromParentAndCleanup(true);
			}), nullptr)
			);
			break;
		case 'W':
			isShoot = false;
			bullet->setRotation(270);
			bullet->runAction(
				Sequence::create(
					MoveBy::create(1.0f, Vec2(0, visibleSize.height)),
					CallFunc::create([=] {
				bullets.remove(bullet);
				temp->removeFromParentAndCleanup(true);
			}), nullptr)
			);
			break;
		case 'S':
			isShoot = false;
			bullet->setRotation(90);
			bullet->runAction(
				Sequence::create(
					MoveBy::create(1.0f, Vec2(0, -visibleSize.height)),
					CallFunc::create([=] {
				bullets.remove(bullet);
				temp->removeFromParentAndCleanup(true);
			}), nullptr)
			);
			break;
		}
		return;
	}
}

// 自定义碰撞事件
void GameSence::meet(EventCustom * event) {
	cocos2d::Vector<SpriteFrame*> explore;
	auto texture = Director::getInstance()->getTextureCache()->addImage("explosion.png");
	explore.reserve(7);
	for (int i = 0; i < 5; i++) {
		auto frame = SpriteFrame::createWithTexture(texture, CC_RECT_PIXELS_TO_POINTS(Rect(190 * i + 2, 0, 188, 210)));
		explore.pushBack(frame);
	}
	auto frame = SpriteFrame::createWithTexture(texture, CC_RECT_PIXELS_TO_POINTS(Rect(0, 210, 190, 210)));
	explore.pushBack(frame);
	frame = SpriteFrame::createWithTexture(texture, CC_RECT_PIXELS_TO_POINTS(Rect(190, 210, 190, 210)));
	explore.pushBack(frame);

	list<Sprite*>::iterator it1, it2;
	for (it1 = bullets.begin(); it1 != bullets.end(); it1++) {
		for (int i = 0; i < 5; i++) {
			if (surviveEnemy[i] && (*it1)->getPosition().getDistance(enemy[i]->getPosition()) < 25) {
				Sprite* temp = enemy[i];
				temp->runAction(
					Sequence::create(
						Animate::create(
							Animation::createWithSpriteFrames(explore, 0.05f, 1)
						),
						CallFunc::create([temp] {
					temp->removeFromParentAndCleanup(true);
				}),
						nullptr)
				);
				surviveEnemy[i] = false;
				temp = *it1;
				temp->removeFromParentAndCleanup(true);
				bullets.remove(*it1);
				score++;
				char str[15];
				sprintf(str, "    Score: %d", score);
				Score->setString(str);
				SimpleAudioEngine::getInstance()->playEffect("music/explore.wav", false);
				if (score == 5) {
					Sprite* victory = Sprite::create("victory.png");
					victory->setScale(3);
					victory->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
					this->addChild(victory, 3);
				}
				return;
			}
		}
	}
	for (it2 = bullets1.begin(); it2 != bullets1.end(); it2++) {
		if ((*it2)->getPosition().getDistance(player->getPosition()) < 16) {
			
			Sprite* temp = *it2;
			temp->removeFromParentAndCleanup(true);
			bullets1.remove(*it2);
			life--;
			SimpleAudioEngine::getInstance()->playEffect("music/hurt.wav", false);
			char str[15];
			sprintf(str, "Life: %d", life);
			Life->setString(str);
			if (life != 0) return;
			Sprite* p = player;
			p->runAction(
				Sequence::create(
					Animate::create(
						Animation::createWithSpriteFrames(explore, 0.05f, 1)
					),
					CallFunc::create([=] {
				p->removeFromParentAndCleanup(true);
				_eventDispatcher->removeAllEventListeners();
				_scheduler->unscheduleAll();
				SimpleAudioEngine::getInstance()->playEffect("music/explore.wav", false);
				Sprite* gameover = Sprite::create("gameOver.png");
				gameover->setScale(3);
				gameover->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
				this->addChild(gameover, 3);
			}),
					nullptr)
			);

			return;
		}
	}
}

void GameSence::restart(Ref* pSender) {
	auto scene = GameSence::createScene();
	auto slidein = TransitionCrossFade::create(1.0f, scene);
	Director::getInstance()->replaceScene(slidein);
}

void GameSence::movePlane(char c)
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