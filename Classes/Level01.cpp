
#include "Level01.h"
#include "HelloWorldScene.h"

USING_NS_CC;

Scene* Level01::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
	auto layer = Level01::create();

    // add layer as a child to scene
    scene->addChild(layer);
	
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool Level01::init()
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

	setPlayer();
	setBG();
	
	animCache();
	auto shade = DrawNode::create();
	shade->drawSolidRect(Vec2(0, 0), Vec2(m_visibleSize.width, m_visibleSize.height), Color4F(0, 0, 0, 180));
	this->addChild(shade);
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


	m_sadmanMsg = ui::Button::create("img/button.png");
	m_sadmanMsg->setScale(1);
	m_sadmanMsg->setTitleColor(Color3B::WHITE);
	m_sadmanMsg->setTitleFontName("fonts/ant-maru.ttf");
	m_sadmanMsg->setTitleFontSize(26);
	m_sadmanMsg->setTitleText("トナリノヘアガアカルクテ");
	m_sadmanMsg->setVisible(false);
	m_sadmanMsg->setPosition(Vec2(m_visibleSize.width / 2, m_visibleSize.height / 10));
	m_sadmanMsg->setEnabled(false);
	this->addChild(m_sadmanMsg);

	this->scheduleUpdate();
	
    return true;
}

void Level01::setBG()
{
	m_worldMap = Node::create();

	if (UserDefault::getInstance()->getBoolForKey("light"))
	{
		m_collision = m_visibleSize.width*0.3;
	}
	else
	{
		m_collision = -m_visibleSize.width*0.7;
	}

	Sprite* bg[3];

	for (int i = 0; i < 3; i++)
	{
		bg[i] = Sprite::create("img/background.png");

		// position the sprite on the center of the screen
		bg[i]->setScale(m_visibleSize.width / m_visibleSize.height);
		bg[i]->setPosition(Point((m_visibleSize.width) * i+1, m_visibleSize.height / 2));

		// add the sprite as a child to this layer
		m_worldMap->addChild(bg[i]);
	}

	createObjectOnFloor(bg[1]->getBoundingBox().getMidX()- 200, "img/chair.png");
	createObjectOnFloor(bg[1]->getBoundingBox().getMidX(), "img/chair.png");
	createObjectOnFloor(bg[1]->getBoundingBox().getMidX(), "img/table.png");
	createObjectOnFloor(bg[0]->getBoundingBox().getMidX(), "img/chair.png");

	auto drawing2 = Sprite::create("img/drawing01.png");
	drawing2->setScale(2);
	drawing2->setPosition(Vec2(m_visibleSize.width * 1.3, m_visibleSize.height / 1.4));
	//m_worldMap->addChild(drawing2);

	//m_doorList[2] = createDoor(m_visibleSize.width * 2.7);
	m_doorList[0] = createDoor(convertToNodeSpace(sprite->getPosition()).x);
	//m_doorList[1] = createDoor(m_visibleSize.width * 3.4);

	auto sadman  = Sprite::create("img/sadman01.png");
	sadman->setScale(1.8);
	sadman->setPosition(Vec2(m_visibleSize.width * 1.2 + 55, m_floorPoint + sadman->getBoundingBox().getMaxY()));
	m_worldMap->addChild(sadman);
	if (UserDefault::getInstance()->getBoolForKey("light"))
	{
		sadman->setVisible(true);
	}
	else
	{
		sadman->setVisible(false);
	}

	auto sadmanAnim = Animation::create();
	sadmanAnim->addSpriteFrameWithFileName("img/sadman01.png");
	sadmanAnim->addSpriteFrameWithFileName("img/sadman01.png");
	sadmanAnim->addSpriteFrameWithFileName("img/sadman02.png");
	sadmanAnim->addSpriteFrameWithFileName("img/sadman01.png");
	sadmanAnim->addSpriteFrameWithFileName("img/sadman02.png");
	sadmanAnim->addSpriteFrameWithFileName("img/sadman03.png");
	sadmanAnim->addSpriteFrameWithFileName("img/sadman02.png");
	sadmanAnim->addSpriteFrameWithFileName("img/sadman03.png");
	sadmanAnim->addSpriteFrameWithFileName("img/sadman02.png");
	sadmanAnim->addSpriteFrameWithFileName("img/sadman03.png");
	sadmanAnim->addSpriteFrameWithFileName("img/sadman02.png");
	sadmanAnim->addSpriteFrameWithFileName("img/sadman03.png");
	sadmanAnim->addSpriteFrameWithFileName("img/sadman04.png");
	sadmanAnim->addSpriteFrameWithFileName("img/sadman04.png");
	sadmanAnim->addSpriteFrameWithFileName("img/sadman04.png");
	sadmanAnim->setLoops(-1);
	sadmanAnim->setDelayPerUnit(0.3f);
	sadmanAnim->setRestoreOriginalFrame(true);

	auto sadmanAnimate = Animate::create(sadmanAnim);
	sadman->runAction(RepeatForever::create(sadmanAnimate));

	m_doorList[1] = sadman;

	this->addChild(m_worldMap);
}

cocos2d::Label* Level01::createMsg(float posX, float posY, std::string text, int fontSize, cocos2d::Color3B color)
{
	auto msg = Label::create(text, "fonts/ant-maru.ttf", fontSize);
	msg->setPosition(Vec2(posX, posY));
	msg->setColor(color);
	m_worldMap->addChild(msg);

	return msg;
}

cocos2d::Sprite* Level01::createDoor(float posX)
{
	auto door = Sprite::create("img/door.png");
	door->setScale(1.8);
	door->setPosition(Vec2(posX, m_floorPoint + door->getBoundingBox().getMaxY()));
	m_worldMap->addChild(door);

	return door;
}

cocos2d::Sprite* Level01::createObjectOnFloor(float posX, std::string path, float scale)
{
	auto object = Sprite::create(path);
	object->setScale(scale);
	object->setPosition(posX, m_floorPoint + object->getBoundingBox().getMaxY());
	m_worldMap->addChild(object);

	return object;
}

void Level01::setPlayer()
{
	sprite = Sprite::create("img/player01.png");
	sprite->setScale(2);
	sprite->setPosition(Vec2(m_visibleSize.width / 2, m_visibleSize.height / 2.52));
	sprite->setZOrder(100);
	this->addChild(sprite);
}

void Level01::update(float dt)
{
	if (m_worldMap->convertToWorldSpace(Vec2(getBoundingBox().getMaxX(), getBoundingBox().getMinY())).x < m_collision)
	{
		m_worldMap->stopAllActions();
		m_worldMap->setPositionX(m_worldMap->getBoundingBox().getMaxX() + 10);
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
	else if (m_worldMap->convertToWorldSpace(m_doorList[1]->getPosition()).x <= playerRect.getMaxX() + 30 && m_worldMap->convertToWorldSpace(m_doorList[1]->getPosition()).x >= playerRect.getMinX())
	{
		eventFlag(1);
	}
	else
	{
		m_enterButton->setVisible(false);
		m_sadmanMsg->setVisible(false);
	}
	
}

void Level01::animCache()
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

void Level01::eventHandler()
{
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(Level01::onTouchBegan, this);
	listener->onTouchEnded = CC_CALLBACK_2(Level01::onTouchEnded, this);

	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
}

void Level01::walk(int dir)
{
	m_moveRight = RepeatForever::create(MoveBy::create(0.5f, Vec2(80, 0)));
	m_moveLeft = MoveBy::create(0.5f, Vec2(-80, 0));

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

bool Level01::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* eventt)
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

void Level01::onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* eventt)
{
	idle();
}

void Level01::stopActions()
{
	sprite->stopAllActions();
	m_worldMap->stopAllActions();
}

void Level01::eventFlag(int eventNumber)
{
	switch (eventNumber)
	{
	case 0:
		m_enterButton->setVisible(true);
		m_enterButton->addClickEventListener(CC_CALLBACK_1(Level01::enterDoor, this, eventNumber));
		break;
	case 1:
		if (m_doorList[1]->isVisible())
		{
			m_sadmanMsg->setVisible(true);
		}
		//m_sadmanMsg->addClickEventListener(CC_CALLBACK_1(Level01::enterDoor, this, eventNumber));
		break;
	case 2:
		m_enterButton->addClickEventListener(CC_CALLBACK_1(Level01::enterDoor, this, eventNumber));
		break;
	default:
		break;
	}
}

void Level01::idle()
{
	stopActions();

	auto standRight = Animate::create(m_animCache->getAnimation("stand"));
	sprite->runAction(RepeatForever::create(standRight));
}

void Level01::enterDoor(cocos2d::Ref* pSender, int doorNumber)
{
	Scene* nextScene;
	switch (doorNumber)
	{
	case 0:
		UserDefault::getInstance()->setIntegerForKey("forRoom1", 1);
		nextScene = HelloWorld::createScene();
		break;
	case 1:
		nextScene = HelloWorld::createScene();
		break;
	default:
		nextScene = Level01::createScene();
		break;
	}
	
	auto pScene = TransitionFade::create(0.5f, nextScene);
	Director::getInstance()->replaceScene(pScene);
}

void Level01::menuCloseCallback(Ref* pSender)
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