#include "GameScene.h"
#include "VisibleRect.h"
#include "AppMacros.h"

using namespace std;

enum{
	kNodeTag = 1,
};
CCScene* GameLayer::createScene()
{
	CCScene* scene = CCScene::create();

	GameLayer* layer = GameLayer::create();

	scene->addChild(layer);

	CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("selected_new.wav");
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

	CCSpriteBatchNode* node = CCSpriteBatchNode::create("my_cats.png");
	this->addChild(node, 0, kNodeTag);
	/*Cat::cache = CCSpriteFrameCache::sharedSpriteFrameCache();
	Cat::cache->addSpriteFramesWithFile("my_cats.plist");*/
	Cat::initCache();

	m_times = 0;
	highestScore = 322654;
	level = 1;
	targetScore = levelScore[level-1];
	currentScore = 0;
	justScore = 0;

	//top label
	CCNode* topNode = CCLayerColor::create(ccc4(0,0,0,88));
	topNode->setContentSize(CCSizeMake(VisibleRect::right().x, catSize));
	topNode->setAnchorPoint(ccp(0,0));
	topNode->setPosition(ccp(0,VisibleRect::top().y-catSize));
	this->addChild(topNode);

	highestScoreNode = CCLabelTTF::create("最高分","arial", 24);
	highestScoreNode->setColor(ccc3(255,255,255));
	string highestStr = WStrToUTF8(L"最高分 ");
	ostringstream os;
	os<<highestScore;
	highestStr = highestStr + os.str();
	highestScoreNode->setString(highestStr.c_str());
	highestScoreNode->setPosition(ccp(highestScoreNode->getContentSize().width/2 + 10, 55));
	topNode->addChild(highestScoreNode);

	targetScoreNode = CCLabelTTF::create("目标分","arial", 24);
	targetScoreNode->setColor(ccc3(255,255,255));
	string targetStr = WStrToUTF8(L"目标分 ");
	os.str("");
	os<<targetScore;
	targetStr = targetStr + os.str();
	targetScoreNode->setString(targetStr.c_str());
	targetScoreNode->setPosition(ccp(VisibleRect::center().x, 55));
	topNode->addChild(targetScoreNode);

	levelNode = CCLabelTTF::create("关卡","arial", 24);
	levelNode->setColor(ccc3(128,128,255));
	string levelStr = WStrToUTF8(L"关卡 ");
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
	currentScoreNode->setString("0");
	currentScoreNode->setPosition(ccp(VisibleRect::center().x, currentScoreNode->getContentSize().height/2));
	topNode->addChild(currentScoreNode);

	hintScoreNode = CCLabelTTF::create();
	hintScoreNode->setColor(ccc3(255,255,255));
	hintScoreNode->setFontName("arial");
	hintScoreNode->setFontSize(20);
	hintScoreNode->setString("n blocks 5*n*n points");
	hintScoreNode->setPosition(ccp(VisibleRect::center().x, VisibleRect::top().y - 120));
	this->addChild(hintScoreNode);
	hintScoreNode->setVisible(false);

	s_levelLabel = CCLabelTTF::create("关卡","arial", 30);
	s_levelLabel->setColor(ccc3(255,255,255));
	this->addChild(s_levelLabel);

	s_label = CCLabelTTF::create("", "arial", 30);
	s_label->setColor(ccc3(255,255,255));
	this->addChild(s_label);

	s_targetScore = CCLabelTTF::create("", "arial", 30);
	s_targetScore->setColor(ccc3(255,255,255));
	this->addChild(s_targetScore);

	result_label = CCLabelBMFont::create("win", "fonts/bitmapFontTest.fnt");
	result_label->setScale(1.5f);
	result_label->setPosition(ccp(VisibleRect::center().x, VisibleRect::top().y+result_label->getContentSize().height));
	this->addChild(result_label);

	initLevelState();

	return true;
}

void GameLayer::initLevelState()
{
	ostringstream os;
	string levelStr = WStrToUTF8(L"关 卡 ");
	os<<level;
	levelStr = levelStr + os.str();
	levelNode->setString(levelStr.c_str());
	s_levelLabel->setString(levelStr.c_str());
	s_levelLabel->setPosition(ccp(0-VisibleRect::center().x, VisibleRect::center().y + 50));

	string label_target = WStrToUTF8(L"目 标 分");
	s_label->setString(label_target.c_str());
	s_label->setPosition(ccp(0-VisibleRect::center().x, VisibleRect::center().y));

	os.str("");
	os<<targetScore;
	s_targetScore->setString(os.str().c_str());
	s_targetScore->setPosition(ccp(0-VisibleRect::center().x, VisibleRect::center().y - 50));

	string targetAndScore = WStrToUTF8(L"目标分 ");
	targetAndScore = targetAndScore + os.str();
	targetScoreNode->setString(targetAndScore.c_str());

	os.str("");
	os<<currentScore;
	currentScoreNode->setString(os.str().c_str());

	CCActionInterval* moveIn = CCMoveBy::create(0.5f, ccp(VisibleRect::right().x, 0));
	CCActionInterval* moveOut = CCMoveBy::create(0.5f, ccp(VisibleRect::right().x, 0));

	CCActionInterval* moveIn_1 = CCEaseElasticOut::create(((CCActionInterval*)moveIn->copy()->autorelease()), 0.5f);
	CCActionInterval* moveOut_1 = (CCActionInterval*)moveOut->copy()->autorelease();

	CCActionInterval* moveIn_2 = CCEaseElasticOut::create(((CCActionInterval*)moveIn->copy()->autorelease()), 0.8f);
	CCActionInterval* moveOut_2 = (CCActionInterval*)moveOut->copy()->autorelease();

	CCDelayTime *delay = CCDelayTime::create(0.5f);
	
	CCSequence* sq = CCSequence::create(moveIn, delay, moveOut, CCCallFunc::create(this, callfunc_selector(GameLayer::initData)), NULL);
	CCSequence* sq1 = CCSequence::create(moveIn_1, CCCA(delay), moveOut_1, NULL);
	CCSequence* sq2 = CCSequence::create(moveIn_2, CCCA(delay), moveOut_2, NULL);

	s_targetScore->runAction(sq);
	s_label->runAction(sq1);
	s_levelLabel->runAction(sq2);

	this->schedule(schedule_selector(GameLayer::initFinished), 2.8f);
}

void GameLayer::initData()
{
	m_content = new CCArray*[boxSize];
	CCNode* parent = getChildByTag(kNodeTag);
	parent->removeAllChildren();
	for(int i=0; i<boxSize; i++)
	{
		m_content[i] = CCArray::createWithCapacity(boxSize);
		for(int j=0; j<boxSize; j++)
		{
			int random = getRandomColor();
			Cat* cat = Cat::CreateWithNum(random);
			cat->setXandY(i,j);
			cat->setAnchorPoint(ccp(0,0));
			cat->setPosition(ccp(i*catSize, VisibleRect::top().y + j*catSize));
			parent->addChild(cat);
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
					tempCat->setStatus(2);
					tempCat->removeFromParentAndCleanup(true);
					//TODO 粒子效果 声音
					playParticleEffect(ccp(tempCat->x*catSize + catSize/2, tempCat->y*catSize + catSize/2));
				}
				updateContent();
				updateViewByContent();
				m_selected->release();
				m_selected = NULL;
				hintScoreNode->setVisible(false);
				if(isGameEnd())
				{
					processGameEnd();
				}
				return;
			}else
			{
				CCObject* pObj;
				CCARRAY_FOREACH(m_selected, pObj)
				{
					Cat* tempCat = (Cat*)pObj;
					tempCat->setToSelected(false);
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
					tempCat->setToSelected(true);
				}

				//选中音效
				CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("selected_new.wav");
				int n = m_selected->count();
				char string[50] = {0};
				sprintf(string, "%d blocks %d points", n, getScoreByNum(n));
				hintScoreNode->setString(string);
				hintScoreNode->setScale(0.5f);
				CCActionInterval* scale = CCScaleTo::create(0.4f, 1.5f);
				CCActionInterval* scale_ease_out = CCEaseElasticOut::create(((CCActionInterval*)scale->copy()->autorelease()));
				hintScoreNode->setVisible(true);
				hintScoreNode->runAction(scale_ease_out);
			}else
			{
				m_selected->release();
				m_selected = NULL;
				hintScoreNode->setVisible(false);
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

void GameLayer::playParticleEffect(CCPoint point)
{
	CCParticleSystemQuad *particle = CCParticleFlower::create();
	particle->setTexture(CCTextureCache::sharedTextureCache()->addImage("fire.png"));
	particle->setDuration(0.3f);
	particle->setGravity(ccp(0,-100));
	particle->setLifeVar(0);
	particle->setLife(1.0f);
	particle->setSpeed(100);
	particle->setSpeedVar(40);
	particle->setEmissionRate(500);
	particle->setAutoRemoveOnFinish(true);
	this->addChild(particle, 10);
	particle->setPosition(point);
}
void GameLayer::updateContent()
{
	justScore = currentScore;
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
	if(m_times == 0)
	{
		schedule(schedule_selector(GameLayer::stepForScore));
	}else
	{
		m_times = 0;
		unschedule(schedule_selector(GameLayer::stepForScore));
		schedule(schedule_selector(GameLayer::stepForScore));
	}
	
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

void GameLayer::stepForScore(float dt)
{
	int score = justScore + (currentScore - justScore) * m_times/60;
	char string[12] = {0};
	sprintf(string, "%d", score);
	currentScoreNode->setString(string);
	if(m_times == 60)
	{
		unschedule(schedule_selector(GameLayer::stepForScore));
		m_times = 0;
		return;
	}
	m_times++;
}

int GameLayer::getRandomColor()
{
	cc_timeval psv;
	CCTime::gettimeofdayCocos2d(&psv,NULL);
	unsigned long int seed = psv.tv_sec*1000 + psv.tv_usec*1000;
	srand(seed);
	int random = (int)(CCRANDOM_0_1() * 100)%5;
	return random;
}

bool GameLayer::isGameEnd()
{
	
	for(int i=0;i<boxSize;i++)
	{
		CCObject* pObj;
		CCARRAY_FOREACH(m_content[i], pObj)
		{
			CCArray* array = CCArray::create();
			popCat(pObj, array);
			if(array->count() > 1)
			{
				return false;
			}
		}
	}
	
	return true;
}

void GameLayer::removeLeftCat()
{
	for(int i=0; i<boxSize; i++)
	{
		CCObject* pObj;
		CCARRAY_FOREACH(m_content[i], pObj)
		{
			Cat* tempCat = (Cat*)pObj;
			tempCat->removeFromParentAndCleanup(true);
			playParticleEffect(ccp(tempCat->x*catSize + catSize/2, tempCat->y*catSize + catSize/2));
		}
	}

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
}

void GameLayer::processLeftCat()
{
	for(int i=0; i<boxSize; i++)
	{
		CCObject* pObj;
		CCARRAY_FOREACH(m_content[i], pObj)
		{
			Cat* tempCat = (Cat*)pObj;
			tempCat->setStatus(3);
		}
	}
}

void GameLayer::processGameEnd()
{
	if(currentScore >= targetScore)
	{
		level++;
		if(level > 9)
		{
			return;
		}
		targetScore = levelScore[level-1];
		result_label->setString("win");
	}else
	{
		level = 1;
		targetScore = levelScore[level-1];
		currentScore = 0;
		justScore = 0;
		result_label->setString("game over");
	}
	this->setTouchEnabled(false);

	CCDelayTime *delay1 = CCDelayTime::create(1.5f);
	CCDelayTime *delay = CCDelayTime::create(0.5f);
	CCCallFunc* processCat = CCCallFunc::create(this, callfunc_selector(GameLayer::processLeftCat));
	CCCallFunc* initLevel = CCCallFunc::create(this, callfunc_selector(GameLayer::initLevelState));
	CCCallFunc* removeCat = CCCallFunc::create(this, callfunc_selector(GameLayer::removeLeftCat));

	CCActionInterval* moveIn = CCMoveTo::create(0.4f, VisibleRect::center());
	CCActionInterval* moveOut = CCMoveTo::create(0.4f, ccp(VisibleRect::center().x, 
		VisibleRect::top().y + result_label->getContentSize().height));
	
	CCSequence* sq = CCSequence::create(delay1, processCat, delay, removeCat, CCCA(delay1), 
		moveIn, CCCA(delay), moveOut, CCCA(delay), initLevel, NULL);
	result_label->runAction(sq);
}