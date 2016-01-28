#ifndef __TitleScene__
#define __TitleScene__

#include "cocos2d.h"

class TitleScene : public cocos2d::Layer
{
private:
	cocos2d::Size m_visibleSize;

public:
	static cocos2d::Scene* createScene();

	virtual bool init();
	void displayBG();

	void menuSceneTransition(cocos2d::Ref* pSender);

	CREATE_FUNC(TitleScene);
};

#endif