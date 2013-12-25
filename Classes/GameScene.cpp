#include "GameScene.h"
#include "VisibleRect.h"
#include "AppMacros.h"

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
	if(m_content != NULL)
	{
		for(int i=0; i<boxSize; i++)
		{
			if(m_content[i] != NULL)
			{
				m_content[i]->release();
			}
		}
	}
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

	//top label
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

	targetScoreNode = CCLabelTTF::create("目标分","arial", 24);
	targetScoreNode->setColor(ccc3(255,255,255));
	string targetStr = WStrToUTF8(L"目标分 ");
	level = 1;
	targetScore = levelScore[level-1];
	os.str("");
	os<<targetScore;
	targetStr = targetStr + os.str();
	targetScoreNode->setString(targetStr.c_str());
	targetScoreNode->setPosition(ccp(VisibleRect::center().x, 55));
	topNode->addChild(targetScoreNode);

	levelNode = CCLabelTTF::create("关卡","arial", 24);
	levelNode->setColor(ccc3(128,128,255));
	string levelStr = WStrToUTF8(L"关卡 ");
	level = 1;
	os.str("");
	os<<level;
	levelStr = levelStr + os.str();
	levelNode->setString(levelStr.c_str());
	levelNode->setPosition(ccp(VisibleRect::right().x*3/4,55));
	topNode->addChild(levelNode);

	currentScoreNode = CCLabelTTF::create();
	currentScoreNode->setColor(ccc3(255,255,255));
	currentScoreNode->setFontName("arial");
	currentScoreNode->setFontSize(26);
	currentScore = 0;
	currentScoreNode->setString("0");
	currentScoreNode->setPosition(ccp(VisibleRect::center().x, currentScoreNode->getContentSize().height/2));
	topNode->addChild(currentScoreNode);

	initData();
	this->schedule(schedule_selector(GameLayer::initFinished), 1.3f);

	return true;
}

void GameLayer::initData()
{
	m_content = new CCArray*[boxSize];
	for(int i=0; i<boxSize; i++)
	{
		m_content[i] = CCArray::createWithCapacity(boxSize);
		for(int j=0; j<boxSize; j++)
		{
			int random = (int)(CCRANDOM_0_1() * 100)%6;
			Cat* cat = Cat::CreateWithNum(random);
			cat->setXandY(i,j);
			cat->setAnchorPoint(ccp(0,0));
			cat->setPosition(ccp(i*catSize, VisibleRect::top().y + j*catSize));
			this->addChild(cat);
			CCActionInterval* actionTo = CCMoveTo::create(0.8+0.06*j, ccp(i*catSize, j*catSize));
			m_content[i]->addObject(cat);
			cat->runAction(actionTo);
		}
		m_content[i]->retain();
		
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
		int temp = m_content[tx]->count();
		if(ty >= m_content[tx]->count())
		{
			return;
		}

		if(m_selected != NULL)
		{
			if(m_selected->containsObject(m_content[tx]->objectAtIndex(ty)))
			{
				// 消除
				CCObject* obj;
				CCARRAY_FOREACH(m_selected, obj)
				{
					Cat* tempCat = (Cat*)obj;
					m_content[tempCat->x]->removeObject(tempCat);
					tempCat->status = 2;
					tempCat->removeFromParentAndCleanup(true);
					//TODO 粒子效果 声音
				}
				updateContent();
				updateViewByContent();
				m_selected->release();
				m_selected = NULL;
				return;
			}else
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
			
		}
		
		if(ty < m_content[tx]->count())
		{
			CCArray* array = CCArray::create();
			popCat(m_content[tx]->objectAtIndex(ty), array);

			m_selected = (CCArray*)array->copy();
			if(m_selected->count() > 1)
			{
				CCObject* pObj;
				CCARRAY_FOREACH(m_selected, pObj)
				{
					Cat* tempCat = (Cat*)pObj;
					tempCat->status = 1;
					//选中音效
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

void GameLayer::popCat(CCObject* object, CCArray* array)
{
	array->addObject(object);
	Cat* cat = (Cat*)object;
	int ax = cat->x;
	int ay = cat->y;

	if(ax > 0 && ay < m_content[ax-1]->count() 
		&& ((Cat*)m_content[ax-1]->objectAtIndex(ay))->catColor == cat->catColor)
	{
		if(!array->containsObject(m_content[ax-1]->objectAtIndex(ay)))
		{
			popCat(m_content[ax-1]->objectAtIndex(ay), array);
		}
	}
	if(ax < boxSize-1 && ay < m_content[ax+1]->count()
		&& ((Cat*)m_content[ax+1]->objectAtIndex(ay))->catColor == cat->catColor)
	{
		if(!array->containsObject(m_content[ax+1]->objectAtIndex(ay)))
		{
			popCat(m_content[ax+1]->objectAtIndex(ay), array);
		}
	}
	if(ay > 0 && ay-1 < m_content[ax]->count()
		&& ((Cat*)m_content[ax]->objectAtIndex(ay-1))->catColor == cat->catColor)
	{
		if(!array->containsObject(m_content[ax]->objectAtIndex(ay-1)))
		{
			popCat(m_content[ax]->objectAtIndex(ay-1), array);
		}
	}
	if(ay < boxSize-1 && ay+1 < m_content[ax]->count()
		&& ((Cat*)m_content[ax]->objectAtIndex(ay+1))->catColor == cat->catColor)
	{
		if(!array->containsObject(m_content[ax]->objectAtIndex(ay+1)))
		{
			popCat(m_content[ax]->objectAtIndex(ay+1), array);
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
	currentScore = currentScore + getScoreByNum(m_selected->count());

	CCArray* tempArray = CCArray::create();

	for(int i=0; i<boxSize; i++)
	{
		if(m_content[i]->count() > 0)
		{
			tempArray->addObject(m_content[i]);
		}
	}

	if(tempArray->count() < boxSize)
	{
		for(int j=0; j<boxSize; j++)
		{
			if(m_content[j]->count() == 0)
			{
				m_content[j]->release();
			}

			if(j < tempArray->count())
			{
				m_content[j] = (CCArray*)tempArray->objectAtIndex(j);
			}else
			{
				m_content[j] = CCArray::create();
				m_content[j]->retain();
			}	
		}
	}
}

void GameLayer::updateViewByContent()
{
	stringstream ss;
	ss<<currentScore;
	currentScoreNode->setString(ss.str().c_str());
	for(int i=0; i<boxSize; i++)
	{
		if(m_content[i] != NULL)
		{
			CCObject* obj;
			int j=0;
			CCARRAY_FOREACH(m_content[i], obj)
			{
				Cat* tempCat = (Cat*)obj;
				if(tempCat->y!=j || tempCat->x != i)
				{
					tempCat->setXandY(i,j);
					CCActionInterval* actionTo = CCMoveTo::create(0.3, ccp(i*catSize, j*catSize));
					tempCat->runAction(actionTo);
				}
				j++;
			}
		}
	}
}

int GameLayer::getScoreByNum(int num)
{
	return 5 * num*num;
}
