
#include "HelloWorldScene.h"
#include "Level01.h"
#include "Level02.h"
#include "Level03.h"

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
    
	m_visibleSize = Director::getInstance()->getVisibleSize();
	m_floorPoint = m_visibleSize.height / 4.6;
    Point origin = Director::getInstance()->getVisibleOrigin();

	auto wall = DrawNode::create();
	wall->drawSolidRect(Vec2(0, 0), Vec2(m_visibleSize.width, m_visibleSize.height), Color4F::GRAY);
	this->addChild(wall);

	setBG();
	setPlayer();
	animCache();
	auto shade = DrawNode::create();
	shade->drawSolidRect(Vec2(0, 0), Vec2(m_visibleSize.width, m_visibleSize.height), Color4F(0, 0, 0, 180));
	//this->addChild(shade);
	idle();
	eventHandler();
	m_enterButton = ui::Button::create("img/button.png");
	m_enterButton->setScale(0.6);
	m_enterButton->setTitleColor(Color3B::WHITE);
	m_enterButton->setTitleFontName("fonts/ant-maru.ttf");
	m_enterButton->setTitleFontSize(45);
	m_enterButton->setTitleText("入る");
	m_enterButton->setVisible(false);
	m_enterButton->setPosition(Vec2(m_visibleSize.width / 2, m_visibleSize.height / 10));
	this->addChild(m_enterButton);


	this->scheduleUpdate();
	
    return true;
}

void HelloWorld::setBG()
{
	m_worldMap = Node::create();

	Sprite* bg[5];

	for (int i = 0; i < 5; i++)
	{
		bg[i] = Sprite::create("img/background.png");

		// position the sprite on the center of the screen
		bg[i]->setScale(m_visibleSize.width / m_visibleSize.height);
		bg[i]->setPosition(Point((m_visibleSize.width) * i+1, m_visibleSize.height / 2));

		// add the sprite as a child to this layer
		m_worldMap->addChild(bg[i]);
	}

	auto firstMsgPos = bg[0]->getPositionX() - 200;

	auto tmpCpy = createMsg(firstMsgPos, m_visibleSize.height / 1.3, "ヒーローはマヌケだ", 35);
	//tmpCpy->setRotation(10);

	auto title = Sprite::create("img/title.png");
	title->setScale(m_visibleSize.width / m_visibleSize.height);
	title->setPosition(Point(m_visibleSize.width / 2, m_visibleSize.height / 1.3));
	m_worldMap->addChild(title);

	auto arrow = Sprite::create("img/right_arrow.png");
	arrow->setPosition(title->getPositionX() + 40, title->getBoundingBox().getMinY() - 40);
	arrow->setScale(0.2);
	m_worldMap->addChild(arrow);

	createObjectOnFloor(bg[2]->getBoundingBox().getMaxX(), "img/chair.png");
	createObjectOnFloor(bg[3]->getBoundingBox().getMidX(), "img/chair.png");
	createObjectOnFloor(bg[3]->getBoundingBox().getMidX(), "img/table.png");
	

	auto drawing1 = Sprite::create("img/drawing01.png");
	drawing1->setScale(2);
	drawing1->setPosition(Vec2(m_visibleSize.width * 3, m_visibleSize.height / 1.4));
	m_worldMap->addChild(drawing1);

	auto drawing2 = Sprite::create("img/drawing01.png");
	drawing2->setScale(2);
	drawing2->setPosition(Vec2(m_visibleSize.width * 1.3, m_visibleSize.height / 1.4));
	m_worldMap->addChild(drawing2);

	//createMsg(m_visibleSize.width * 1.8, m_visibleSize.height / 1.3, "伝説は語る", 35);
	//createMsg(m_visibleSize.width * 1.8, m_visibleSize.height / 1.5, "「勇者は三番目のドアに入る」", 35);

	m_doorList[1] = createDoor(m_visibleSize.width * 2.7);
	m_doorList[0] = createDoor(m_visibleSize.width * 2.2);
	//m_doorList[2] = createDoor(m_visibleSize.width * 3.4);
	m_doorList[2] = createObjectOnFloor(m_visibleSize.width * 3.4f, "img/cave.png");

	int fromRoom = UserDefault::getInstance()->getIntegerForKey("forRoom1");
	if (fromRoom == 1)
	{
		m_worldMap->setPositionX(-m_visibleSize.width * 1.7);
	}
	else if (fromRoom == 2)
	{
		m_worldMap->setPositionX(-m_visibleSize.width * 2.2);
	}
	else if (fromRoom == 3)
	{
		m_worldMap->setPositionX(-m_visibleSize.width * 3);
	}

	this->addChild(m_worldMap);
}

cocos2d::Label* HelloWorld::createMsg(float posX, float posY, std::string text, int fontSize, cocos2d::Color3B color)
{
	auto msg = Label::create(text, "fonts/ant-maru.ttf", fontSize);
	msg->setPosition(Vec2(posX, posY));
	msg->setColor(color);
	m_worldMap->addChild(msg);

	return msg;
}

cocos2d::Sprite* HelloWorld::createDoor(float posX)
{
	auto door = Sprite::create("img/door.png");
	door->setScale(1.8);
	door->setPosition(Vec2(posX, m_floorPoint + door->getBoundingBox().getMaxY()));
	m_worldMap->addChild(door);

	return door;
}

cocos2d::Sprite* HelloWorld::createObjectOnFloor(float posX, std::string path, float scale)
{
	auto object = Sprite::create(path);
	object->setScale(scale);
	object->setPosition(posX, m_floorPoint + object->getBoundingBox().getMaxY());
	m_worldMap->addChild(object);

	return object;
}

void HelloWorld::setPlayer()
{
	sprite = Sprite::create("img/player01.png");
	sprite->setScale(2);
	sprite->setPosition(Vec2(m_visibleSize.width / 2, m_visibleSize.height / 2.52));

	this->addChild(sprite);
}

void HelloWorld::update(float dt)
{
	if (m_worldMap->getBoundingBox().getMinX() < -m_visibleSize.width*3.3)
	{
		m_worldMap->stopAllActions();
		m_worldMap->setPositionX(-m_visibleSize.width *3.3 + 10);
	}
	else if (m_worldMap->getBoundingBox().getMaxX() > m_visibleSize.width/1.5)
	{
		m_worldMap->stopAllActions();
		m_worldMap->setPositionX((m_visibleSize.width/1.5) - 10);
	}

	Rect playerRect = sprite->getBoundingBox();
	
	if (m_worldMap->convertToWorldSpace(m_doorList[0]->getPosition()).x <= playerRect.getMaxX() && m_worldMap->convertToWorldSpace(m_doorList[0]->getPosition()).x >= playerRect.getMinX())
	{
		eventFlag(0);
	}
	else if (m_worldMap->convertToWorldSpace(m_doorList[1]->getPosition()).x <= playerRect.getMaxX() && m_worldMap->convertToWorldSpace(m_doorList[1]->getPosition()).x >= playerRect.getMinX())
	{
		eventFlag(1);
	}
	else if (m_worldMap->convertToWorldSpace(m_doorList[2]->getPosition()).x <= playerRect.getMaxX() && m_worldMap->convertToWorldSpace(m_doorList[2]->getPosition()).x >= playerRect.getMinX())
	{
		eventFlag(2);
	}
	else
	{
		m_enterButton->setVisible(false);
	}
	
}

void HelloWorld::animCache()
{
	anim1 = Animation::create();
	anim1->addSpriteFrameWithFileName("img/player02.png");
	anim1->addSpriteFrameWithFileName("img/player03.png");
	anim1->addSpriteFrameWithFileName("img/player04.png");
	anim1->setLoops(1);
	anim1->setDelayPerUnit(0.2f);
	anim1->setRestoreOriginalFrame(true);

	auto standAnim = Animation::create();
	standAnim->addSpriteFrameWithFileName("img/player01.png");
	standAnim->addSpriteFrameWithFileName("img/player01-2.png");
	standAnim->addSpriteFrameWithFileName("img/player01-3.png");
	standAnim->setLoops(1);
	standAnim->setDelayPerUnit(0.7f);
	standAnim->setRestoreOriginalFrame(true);

	m_animCache = AnimationCache::getInstance();
	m_animCache->addAnimation(anim1, "walking");
	m_animCache->addAnimation(standAnim, "stand");
}

void HelloWorld::eventHandler()
{
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(HelloWorld::onTouchBegan, this);
	listener->onTouchEnded = CC_CALLBACK_2(HelloWorld::onTouchEnded, this);

	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
}

void HelloWorld::walk(int dir)
{
	m_moveRight = RepeatForever::create(MoveBy::create(0.5f, Vec2(90, 0)));
	m_moveLeft = MoveBy::create(0.5f, Vec2(-90, 0));

	stopActions();
	if (dir == 1)
	{
		sprite->setFlipX(true);
		
		m_worldMap->runAction( m_moveRight);
	}
	else
	{
		sprite->setFlipX(false);
		m_worldMap->runAction(RepeatForever::create(m_moveLeft));
	}
	walkRight = Animate::create(m_animCache->getAnimation("walking"));
	sprite->runAction(RepeatForever::create(walkRight));
}

bool HelloWorld::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* eventt)
{
	Rect rightHalf = Rect(m_visibleSize.width*0.5, 0, m_visibleSize.width, m_visibleSize.height);
	Rect leftHalf = Rect(0, 0, m_visibleSize.width*0.5, m_visibleSize.height);
	Rect touchPoint = Rect(touch->getLocation().x, touch->getLocation().y, 2, 2);
	if (touchPoint.intersectsRect(rightHalf))
	{
		walk(0);
	}
	else if (touchPoint.intersectsRect(leftHalf))
	{
		walk(1);
	}

	return true;
}

void HelloWorld::onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* eventt)
{
	idle();
}

void HelloWorld::stopActions()
{
	sprite->stopAllActions();
	m_worldMap->stopAllActions();
}

void HelloWorld::eventFlag(int eventNumber)
{
	m_enterButton->setVisible(true);

	switch (eventNumber)
	{
	case 0:
		m_enterButton->addClickEventListener(CC_CALLBACK_1(HelloWorld::enterDoor, this, eventNumber));
		break;
	case 1:
		m_enterButton->addClickEventListener(CC_CALLBACK_1(HelloWorld::enterDoor, this, eventNumber));
		break;
	case 2:
		m_enterButton->addClickEventListener(CC_CALLBACK_1(HelloWorld::enterDoor, this, eventNumber));
		break;
	default:
		break;
	}
}

void HelloWorld::idle()
{
	stopActions();

	auto standRight = Animate::create(m_animCache->getAnimation("stand"));
	sprite->runAction(RepeatForever::create(standRight));
}

void HelloWorld::enterDoor(cocos2d::Ref* pSender, int doorNumber)
{
	Scene* nextScene;
	switch (doorNumber)
	{
	case 0:
		nextScene = Level01::createScene();
		break;
	case 1:
		nextScene = Level02::createScene();
		break;
	case 2:
		nextScene = Level03::createScene();
		break;
	default:
		nextScene = HelloWorld::createScene();
		break;
	}
	
	auto pScene = TransitionFade::create(0.5f, nextScene);
	Director::getInstance()->replaceScene(pScene);
}

void HelloWorld::menuCloseCallback(Ref* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.", "Alert");
	return;
#endif

	Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}