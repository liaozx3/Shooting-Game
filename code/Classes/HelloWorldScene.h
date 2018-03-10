#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
using namespace cocos2d;

class HelloWorld : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    
	void update(float f);
	void menuStartCallback(Ref* pSender);
	void movePlane(char c);

	void Aaction(Ref * pSender);
	void Daction(Ref* pSender);
	void Waction(Ref* pSender);
	void Saction(Ref* pSender);

	void addKeyboardListener();

	void onKeyPressed(EventKeyboard::KeyCode code, Event* event);
	void onKeyReleased(EventKeyboard::KeyCode code, Event* event);
    
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
private:
	cocos2d::Sprite* player, *bg, *bg1, *bg2, *bg3, *start;
	cocos2d::MenuItemImage* startItem;
	cocos2d::Size visibleSize;
	cocos2d::Vec2 origin;
	char movekey;
	bool isMove;
	cocos2d::Vector<SpriteFrame*> runNorth;
	cocos2d::Vector<SpriteFrame*> runEast;
	cocos2d::Vector<SpriteFrame*> runWest;
	cocos2d::Vector<SpriteFrame*> runSouth;
};

#endif // __HELLOWORLD_SCENE_H__
