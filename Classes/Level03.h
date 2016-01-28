
#ifndef __LEVEL03_SCENE_H__
#define __LEVEL03_SCENE_H__

#include "cocos2d.h"
#include "ui\CocosGUI.h"
#include <string>

class Level03 : public cocos2d::Layer
{
private:
	cocos2d::Size m_visibleSize;
	float m_floorPoint;

	cocos2d::ui::Button* m_enterButton;

	cocos2d::Node* m_worldMap;

	cocos2d::Animation* anim1;

	cocos2d::Sprite* sprite;
	cocos2d::Animate* walkRight;
	cocos2d::AnimationCache* m_animCache;
	cocos2d::RepeatForever* m_moveRight;
	cocos2d::MoveBy* m_moveLeft;

	cocos2d::Sprite* m_doorList[3];
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  

	void eventHandler();
	void walk(int dir);
	void idle();
	void eventFlag(int eventNumber);

	void animCache();
	void stopActions();
	void setBG();
	void setPlayer();

	cocos2d::Label* createMsg(float posX, float posY, std::string text, int fontSize, cocos2d::Color3B color = cocos2d::Color3B::BLACK);
	cocos2d::Sprite* createDoor(float posX);
	cocos2d::Sprite* createObjectOnFloor(float posX, std::string path, float scale = 2.0f);

	void update(float dt) override;

	bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* eventt);
	void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* eventt);
    
    // callbacks
    void menuCloseCallback(cocos2d::Ref* pSender);
	void enterDoor(cocos2d::Ref* pSender, int doorNumber);
    
	CREATE_FUNC(Level03);
};

#endif
