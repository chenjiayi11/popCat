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
	m_selected = NULL;
}

GameLayer::~GameLayer()
{
	delete m_content;
	m_content = NULL;
	if(m_selected != NULL)
	{
		m_selected->release();
	}
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
			m_content[x][y]->setXandY(x, y);
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

	CCSetIterator it = pTouches->begin();
	CCTouch* touch = (CCTouch*)(*it);
	CCPoint location = touch->getLocation();
	if(location.y <= 8*catSize)
	{
		//消除上次选中的效果
		if(m_selected != NULL)
		{
			CCObject* pObj;
			CCARRAY_FOREACH(m_selected, pObj)
			{
				Cat* tempCat = (Cat*)pObj;
				tempCat->status = 0;
			}
			m_selected->release();
			m_selected = NULL;
		}

		int tx = location.x/catSize;
		int ty = location.y/catSize;
		if(m_content[tx][ty] != NULL)
		{
			CCArray* array = CCArray::create();
			popCat(m_content[tx][ty], array);

			m_selected = (CCArray*)array->copy();
		}
		
	}

}

void GameLayer::ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent)
{
}

void GameLayer::ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent)
{
}

void GameLayer::popCat(Cat* cat, CCArray* array)
{
	cat->status = 1;
	array->addObject(cat);
	int ax = cat->x;
	int ay = cat->y;

	if(ax > 0 && m_content[ax-1][ay] != NULL && m_content[ax-1][ay]->catColor == cat->catColor)
	{
		if(array->indexOfObject(m_content[ax-1][ay]) == CC_INVALID_INDEX)
		{
			popCat(m_content[ax-1][ay], array);
		}
	}
	if(ax < boxSize-1 && m_content[ax+1][ay] != NULL && m_content[ax+1][ay]->catColor == cat->catColor)
	{
		if(array->indexOfObject(m_content[ax+1][ay]) == CC_INVALID_INDEX)
		{
			popCat(m_content[ax+1][ay], array);
		}
	}
	if(ay > 0 && m_content[ax][ay-1] != NULL && m_content[ax][ay-1]->catColor == cat->catColor)
	{
		if(array->indexOfObject(m_content[ax][ay-1]) == CC_INVALID_INDEX)
		{
			popCat(m_content[ax][ay-1], array);
		}
	}
	if(ay < boxSize-1 && m_content[ax][ay+1] != NULL && m_content[ax][ay+1]->catColor == cat->catColor)
	{
		if(array->indexOfObject(m_content[ax][ay+1]) == CC_INVALID_INDEX)
		{
			popCat(m_content[ax][ay+1], array);
		}
	}
}