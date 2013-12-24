#include "GameScene.h"
#include "VisibleRect.h"
#include "AppMacros.h"

USING_NS_CC;
using namespace std;

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

	//添加头顶的分数栏
	CCNode* topNode = CCLayerColor::create(ccc4(0,0,0,88));
	topNode->setContentSize(CCSizeMake(VisibleRect::right().x, catSize));
	topNode->setAnchorPoint(ccp(0,0));
	topNode->setPosition(ccp(0,VisibleRect::top().y-catSize));
	this->addChild(topNode);

	highestScoreNode = CCLabelTTF::create("最高分","arial", 24);
	highestScoreNode->setColor(ccc3(255,255,255));
	string highestStr = WStrToUTF8(L"最高分 ");
	highestScore = 322654;
	ostringstream os;
	os<<highestScore;
	highestStr = highestStr + os.str();
	highestScoreNode->setString(highestStr.c_str());
	highestScoreNode->setPosition(ccp(highestScoreNode->getContentSize().width/2 + 10, 55));
	topNode->addChild(highestScoreNode);

	targetScoreNode =  CCLabelTTF::create("目标分","arial", 24);
	targetScoreNode->setColor(ccc3(255,255,255));
	string targetStr = WStrToUTF8(L"目标分 ");
	targetScore = levelScore[1];
	os.str("");
	os<<targetScore;
	targetStr = targetStr + os.str();
	targetScoreNode->setString(targetStr.c_str());
	targetScoreNode->setPosition(ccp(VisibleRect::center().x, 55));
	topNode->addChild(targetScoreNode);

	levelNode = CCLabelTTF::create("关卡","arial", 24);
	levelNode->setColor(ccc3(128,128,255));
	string levelStr = WStrToUTF8(L"关卡 ");
	level = 2;
	os.str("");
	os<<level;
	levelStr = levelStr + os.str();
	levelNode->setString(levelStr.c_str());
	levelNode->setPosition(ccp(VisibleRect::right().x*3/4,55));
	topNode->addChild(levelNode);

	initData();
	this->schedule(schedule_selector(GameLayer::initFinished), 1.3f);

	return true;
}

void GameLayer::initData()
{
	m_content = new Cat**[boxSize];
	for(int i=0; i<boxSize; i++)
	{
		m_content[i] = new Cat*[boxSize];
	}
	for(int y=0; y <boxSize; y++)
	{
		for(int x=0; x<boxSize; x++)
		{
			int random = (int)(CCRANDOM_0_1() * 100)%6;
			m_content[x][y] = Cat::CreateWithNum(random);
			m_content[x][y]->setXandY(x, y);
			m_content[x][y]->setAnchorPoint(ccp(0,0));
			m_content[x][y]->setPosition(ccp(x*catSize, VisibleRect::top().y + y*catSize));
			this->addChild(m_content[x][y]);
			CCActionInterval* actionTo = CCMoveTo::create(0.8+y*0.06, ccp(x*catSize, y*catSize));
			m_content[x][y]->runAction(actionTo);
			/*if(x<boxSize-1 || y<boxSize-1)
			{
				m_content[x][y]->runAction(actionTo);
			}else
			{
				m_content[x][y]->runAction(CCSequence::create(actionTo, 
					CCCallFuncN::create(this, callfuncN_selector(GameLayer::initFinished)),
					NULL));
			}*/
			
		}
	}
}

void GameLayer::initFinished(float dt)
{
	this->setTouchEnabled(true);
	this->unschedule(schedule_selector(GameLayer::initFinished));
}

void GameLayer::ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent)
{

	CCSetIterator it = pTouches->begin();
	CCTouch* touch = (CCTouch*)(*it);
	CCPoint location = touch->getLocation();
	if(location.y <= boxSize*catSize)
	{
		int tx = location.x/catSize;
		int ty = location.y/catSize;
		if(m_content[tx][ty] == NULL)
		{
			return;
		}

		if(m_selected != NULL)
		{
			if(m_selected->containsObject(m_content[tx][ty]))
			{
				//TODO 消除
				CCObject* obj;
				CCARRAY_FOREACH(m_selected, obj)
				{
					Cat* tempCat = (Cat*)obj;
					m_content[tempCat->x][tempCat->y] = NULL;
					tempCat->status = 2;
					tempCat->removeFromParentAndCleanup(true);
				}
				updateContent();
				return;
			}else
			{
				//消除上次选中的效果
				CCObject* pObj;
				CCARRAY_FOREACH(m_selected, pObj)
				{
					Cat* tempCat = (Cat*)pObj;
					tempCat->status = 0;
				}
				m_selected->release();
				m_selected = NULL;
			}
			
		}
		
		if(m_content[tx][ty] != NULL)
		{
			CCArray* array = CCArray::create();
			popCat(m_content[tx][ty], array);

			m_selected = (CCArray*)array->copy();
			if(m_selected->count() > 1)
			{
				CCObject* pObj;
				CCARRAY_FOREACH(m_selected, pObj)
				{
					Cat* tempCat = (Cat*)pObj;
					tempCat->status = 1;
					//播放选中音效
				}
			}else
			{
				m_selected->release();
				m_selected = NULL;
			}
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

void GameLayer::WStrToUTF8(string& dest, const wstring& src)
{
	dest.clear();
	for (size_t i = 0; i < src.size(); i++){
		wchar_t w = src[i];
		if (w <= 0x7f)
			dest.push_back((char)w);
		else if (w <= 0x7ff){
			dest.push_back(0xc0 | ((w >> 6)& 0x1f));
			dest.push_back(0x80| (w & 0x3f));
		}
		else if (w <= 0xffff){
			dest.push_back(0xe0 | ((w >> 12)& 0x0f));
			dest.push_back(0x80| ((w >> 6) & 0x3f));
			dest.push_back(0x80| (w & 0x3f));
		}
		else if (sizeof(wchar_t) > 2 && w <= 0x10ffff){
			dest.push_back(0xf0 | ((w >> 18)& 0x07)); // wchar_t 4-bytes situation
			dest.push_back(0x80| ((w >> 12) & 0x3f));
			dest.push_back(0x80| ((w >> 6) & 0x3f));
			dest.push_back(0x80| (w & 0x3f));
		}
		else
			dest.push_back('?');
	}
}
//! simple warpper
string GameLayer::WStrToUTF8(const wstring& str)
{
	string result;
	WStrToUTF8(result, str);
	return result;
}

void GameLayer::updateContent()
{
	for(int y=0; y<boxSize; y++)
	{
		for(int x=0; x<boxSize; x++)
		{

		}
	}
}

void GameLayer::updateViewByContent()
{

}