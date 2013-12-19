#include "GameScene.h"
#include "VisibleRect.h"
#include "AppMacros.h"

USING_NS_CC;

CCScene* GameLayer::createScene()
{
	CCScene* scene = CCScene::create();

	GameLayer* layer = GameLayer::create();

	scene->addChild(layer);

	return scene;
}

GameLayer::GameLayer()
{
}

GameLayer::~GameLayer()
{
	delete m_content;
	m_content = NULL;
}

bool GameLayer::init()
{
	if(!CCLayer::init())
	{
		return false;
	}

	CCSprite* bg = CCSprite::create("background.png");
	bg->setPosition(VisibleRect::center());

	this->addChild(bg, -1);

	this->setTouchEnabled(true);
	initData();

	return true;
}

void GameLayer::initData()
{
	m_content = new Cat**[8];
	for(int i=0; i<8; i++)
	{
		m_content[i] = new Cat*[8];
	}
	for(int y=0; y <8; y++)
	{
		for(int x=0; x<8; x++)
		{
			int random = (int)(CCRANDOM_0_1() * 100)%6;
			m_content[x][y] = Cat::CreateWithNum(random);
			m_content[x][y]->setAnchorPoint(ccp(0,0));
			m_content[x][y]->setPosition(ccp(x*catSize, VisibleRect::top().y + y*catSize));
			this->addChild(m_content[x][y]);
			CCActionInterval* actionTo = CCMoveTo::create(1+y*0.13, ccp(x*catSize, y*catSize));
			m_content[x][y]->runAction(actionTo);
		}
	}
}

void GameLayer::ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent)
{
	CCScene *pScene = GameLayer::createScene();
	CCDirector::sharedDirector()->replaceScene(pScene);
}

void GameLayer::ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent)
{
}

void GameLayer::ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent)
{
}