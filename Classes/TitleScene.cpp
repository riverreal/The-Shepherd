#include "TitleScene.h"
#include "HelloWorldScene.h"
using namespace cocos2d;
using namespace std;

Scene* TitleScene::createScene() {
	auto scene = Scene::create();
	auto layer = TitleScene::create();
	scene->addChild(layer);
	return scene;
}

bool TitleScene::init()
{
	if (!Layer::init())
	{
		return false;
	}

	//glClearColor(255, 255, 255, 255);

	m_visibleSize = Director::getInstance()->getVisibleSize();
	UserDefault::getInstance()->setBoolForKey("light", true);
	UserDefault::getInstance()->setIntegerForKey("forRoom1", 0);

	displayBG();

	this->scheduleUpdate();

	return true;
}

void TitleScene::displayBG()
{
	auto wall = DrawNode::create();
	wall->drawSolidRect(Vec2(0, 0), Vec2(m_visibleSize.width, m_visibleSize.height), Color4F::WHITE);
	this->addChild(wall);

	auto title = Sprite::create("img/title.png");

	// position the sprite on the center of the screen
	title->setScale(m_visibleSize.width / m_visibleSize.height);
	title->setPosition(Point(m_visibleSize.width / 2, m_visibleSize.height / 1.3));

	// add the sprite as a child to this layer
	this->addChild(title);

	MenuItemFont::setFontSize(60);
	MenuItemFont* newGameItem = MenuItemFont::create("START", CC_CALLBACK_1(TitleScene::menuSceneTransition, this));

	auto label1 = Label::createWithTTF("START", "fonts/arial.ttf", 55);
	label1->setColor(Color3B::BLACK);
	label1->enableOutline(Color4B::BLACK, 2);
	newGameItem->setLabel(label1);

	auto menu = Menu::create(newGameItem, NULL);
	menu->setPosition(Vec2(m_visibleSize.width / 2, m_visibleSize.height / 3.7));
	this->addChild(menu);
}

void TitleScene::menuSceneTransition(cocos2d::Ref *pSender)
{
	auto nextScene = HelloWorld::createScene();
	auto pScene = TransitionFade::create(0.5f, nextScene);
	Director::getInstance()->replaceScene(pScene);
}
