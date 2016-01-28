
#include "Level03.h"

USING_NS_CC;

Scene* Level03::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
	auto layer = Level03::create();

    // add layer as a child to scene
    scene->addChild(layer);
	
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool Level03::init()
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
	wall->drawSolidRect(Vec2(0, 0), Vec2(m_visibleSize.width, m_visibleSize.height), Color4F::BLACK);
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

void Level03::setBG()
{
	m_worldMap = Node::create();

	Sprite* bg[4];

	for (int i = 0; i < 4; i++)
	{
		bg[i] = Sprite::create("img/caveBackground.png");

		// position the sprite on the center of the screen
		bg[i]->setScale(m_visibleSize.width / m_visibleSize.height);
		bg[i]->setPosition(Point((m_visibleSize.width) * i+1, m_visibleSize.height / 2));

		// add the sprite as a child to this layer
		m_worldMap->addChild(bg[i]);
	}

	m_doorList[2] = createDoor(m_visibleSize.width * 2.7);
	m_doorList[0] = createDoor(m_visibleSize.width * 2.2);
	m_doorList[1] = createDoor(m_visibleSize.width * 3.4);

	this->addChild(m_worldMap);
}

cocos2d::Label* Level03::createMsg(float posX, float posY, std::string text, int fontSize, cocos2d::Color3B color)
{
	auto msg = Label::create(text, "fonts/ant-maru.ttf", fontSize);
	msg->setPosition(Vec2(posX, posY));
	msg->setColor(color);
	m_worldMap->addChild(msg);

	return msg;
}

cocos2d::Sprite* Level03::createDoor(float posX)
{
	auto door = Sprite::create("img/door.png");
	door->setScale(1.8);
	door->setPosition(Vec2(posX, m_floorPoint + door->getBoundingBox().getMaxY()));
	m_worldMap->addChild(door);

	return door;
}

cocos2d::Sprite* Level03::createObjectOnFloor(float posX, std::string path, float scale)
{
	auto object = Sprite::create(path);
	object->setScale(scale);
	object->setPosition(posX, m_floorPoint + object->getBoundingBox().getMaxY());
	m_worldMap->addChild(object);

	return object;
}

void Level03::setPlayer()
{
	sprite = Sprite::create("img/player01.png");
	sprite->setScale(2);
	sprite->setPosition(Vec2(m_visibleSize.width / 2, m_visibleSize.height / 2.52));

	this->addChild(sprite);
}

void Level03::update(float dt)
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

void Level03::animCache()
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

void Level03::eventHandler()
{
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(Level03::onTouchBegan, this);
	listener->onTouchEnded = CC_CALLBACK_2(Level03::onTouchEnded, this);

	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
}

void Level03::walk(int dir)
{
	m_moveRight = RepeatForever::create(MoveBy::create(0.5f, Vec2(70, 0)));
	m_moveLeft = MoveBy::create(0.5f, Vec2(-70, 0));

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

bool Level03::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* eventt)
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

void Level03::onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* eventt)
{
	idle();
}

void Level03::stopActions()
{
	sprite->stopAllActions();
	m_worldMap->stopAllActions();
}

void Level03::eventFlag(int eventNumber)
{
	m_enterButton->setVisible(true);

	switch (eventNumber)
	{
	case 0:
		m_enterButton->addClickEventListener(CC_CALLBACK_1(Level03::enterDoor, this, eventNumber));
		break;
	case 1:
		m_enterButton->addClickEventListener(CC_CALLBACK_1(Level03::enterDoor, this, eventNumber));
		break;
	case 2:
		m_enterButton->addClickEventListener(CC_CALLBACK_1(Level03::enterDoor, this, eventNumber));
		break;
	default:
		break;
	}
}

void Level03::idle()
{
	stopActions();

	auto standRight = Animate::create(m_animCache->getAnimation("stand"));
	sprite->runAction(RepeatForever::create(standRight));
}

void Level03::enterDoor(cocos2d::Ref* pSender, int doorNumber)
{
	auto nextScene = Level03::createScene();
	auto pScene = TransitionFade::create(0.5f, nextScene);
	Director::getInstance()->replaceScene(pScene);
}

void Level03::menuCloseCallback(Ref* pSender)
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