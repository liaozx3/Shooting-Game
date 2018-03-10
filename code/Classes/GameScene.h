#ifndef __GAME_SENCE_H__
#define __GAME_SENCE_H__

#include "cocos2d.h"
#include "SimpleAudioEngine.h"
USING_NS_CC;

using namespace std;

class GameSence : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	CREATE_FUNC(GameSence);

	void update(float f);
	void movePlane(char c);

	void addCustomListener();

	void Aaction(Ref * pSender);
	void Daction(Ref* pSender);
	void Waction(Ref* pSender);
	void Saction(Ref* pSender);

	void meet(EventCustom * event);
	void restart(Ref* pSender);

	void addKeyboardListener();
	void preloadMusic();
	void playBgm();
	void onKeyPressed(EventKeyboard::KeyCode code, Event* event);
	void onKeyReleased(EventKeyboard::KeyCode code, Event* event);

private:
	cocos2d::Size visibleSize;
	cocos2d::Vec2 origin;

	int life, score;
	cocos2d::Sprite* player;
	cocos2d::Label* Life, *Score;
	Sprite* enemy[5];
	char movekey;
	bool isMove, isShoot;
	bool surviveEnemy[5];
	int enemyNum;
	int count;

	list<Sprite*> bullets;
	list<Sprite*> bullets1;
	cocos2d::Vector<SpriteFrame*> runNorth;
	cocos2d::Vector<SpriteFrame*> runEast;
	cocos2d::Vector<SpriteFrame*> runWest;
	cocos2d::Vector<SpriteFrame*> runSouth;
};

#endif // __GAME_SENCE_H__
