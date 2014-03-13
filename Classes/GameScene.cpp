#include "GameScene.h"
#include "VisibleRect.h"
#include "AppMacros.h"
#include "MenuScene.h"

#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "platform\android\jni\JniHelper.h"
#endif

using namespace std;

enum{
	kNodeTag = 1,
};
#define SCORE_KEY "highest_score"
#define SQ_TAG 18
#define PERCENTAGE 0.77

CCScene* GameLayer::createScene()
{
	CCScene* scene = CCScene::create();

	GameLayer* layer = GameLayer::create();

	scene->addChild(layer);

	CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("selected.mp3");

	return scene;
}

GameLayer::GameLayer()
{
	m_selected = NULL;
	m_content = NULL;
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
	CocosDenshion::SimpleAudioEngine::sharedEngine()->stopBackgroundMusic(true);
}

void GameLayer::onEnterTransitionDidFinish()
{
	CCLayer::onEnterTransitionDidFinish();
	CCDelayTime* delay = CCDelayTime::create(0.3f);
	CCSequence* squence = CCSequence::createWithTwoActions(delay, CCCallFunc::create(this, callfunc_selector(GameLayer::initLevelState)));
	this->runAction(squence);
}

bool GameLayer::init()
{
	if(!CCLayer::init())
	{
		return false;
	}

	CCSprite* bg_grid = CCSprite::create("bg_grid.png");
	bg_grid->setAnchorPoint(ccp(0,0));
	bg_grid->setPosition(ccp(0,0));
	this->addChild(bg_grid, -2);
	CCSprite* bg_top = CCSprite::create("bg_top.png");
	bg_top->setAnchorPoint(ccp(0,0));
	bg_top->setPosition(ccp(0,bg_grid->getContentSize().height));
	this->addChild(bg_top, 0);

	CCSpriteBatchNode* node = CCSpriteBatchNode::create("my_cats.png");
	this->addChild(node, -1, kNodeTag);
	Cat::initCache();

	m_times = 0;
	highestScore = userDefault->getIntegerForKey(SCORE_KEY);
	level = 1;
	targetScore = levelScore[level-1];
	currentScore = 0;
	justScore = 0;
	isPause = false;

	//top label
	topNode = CCSprite::create("top_node_bg.png");
	int topNodeH = topNode->getContentSize().height;
	topNode->setAnchorPoint(ccp(0,0));
	topNode->setPosition(ccp(0,VisibleRect::top().y-topNodeH));
	this->addChild(topNode);

	targetScoreNode = CCLabelBMFont::create("","score_font.fnt");
	ostringstream os;
	os<<targetScore;
	targetScoreNode->setScale(0.6);
	targetScoreNode->setString(os.str().c_str());
	targetScoreNode->setPosition(ccp(85 + targetScoreNode->getContentSize().width/2, 47-targetScoreNode->getContentSize().height/2));
	topNode->addChild(targetScoreNode);

	levelNode = CCLabelBMFont::create("","shadow_font.fnt");
	os.str("");
	os<<level;
	levelNode->setString(os.str().c_str());
	levelNode->setPosition(ccp(30+levelNode->getContentSize().width/2,topNode->getContentSize().height - 8 - levelNode->getContentSize().height/2));
	topNode->addChild(levelNode);

	currentScoreNode = CCLabelBMFont::create("0", "shadow_font.fnt");
	currentScoreNode->setPosition(ccp(183, 82-currentScoreNode->getContentSize().height/2));
	topNode->addChild(currentScoreNode);

	progressBg = CCSprite::create("progress_bg.png");
	progressBg->setAnchorPoint(ccp(0,1));
	progressBg->setPosition(ccp(183,41));
	topNode->addChild(progressBg);
	progressBar = CCProgressTimer::create(CCSprite::create("progress_bar.png"));
	progressBar->setType(kCCProgressTimerTypeBar);
	progressBar->setBarChangeRate(ccp(1,0));
	progressBar->setMidpoint(ccp(0,0));
	progressBar->setAnchorPoint(ccp(0,1));
	progressBar->setPosition(ccp(183,41));
	progressBar->setPercentage(0);
	topNode->addChild(progressBar);
	spark = CCSprite::create("progress_spark.png");
	spark->setPosition(ccp(183,29));
	spark->setVisible(false);
	topNode->addChild(spark);
	progressEndOff = CCSprite::create("progress_end_off.png");
	progressEndOff->setAnchorPoint(ccp(0,1));
	progressEndOff->setPosition(ccp(463,41));
	topNode->addChild(progressEndOff);
	progressEndOn = CCSprite::create("progress_end_on.png");
	progressEndOn->setAnchorPoint(ccp(0,1));
	progressEndOn->setPosition(ccp(463,41));
	topNode->addChild(progressEndOn);
	progressEndOn->setVisible(false);

	hintScoreNode = CCLabelBMFont::create("", "shadow_font.fnt");
	hintScoreNode->setString("n cats 5*n*n points");
	hintScoreNode->setPosition(ccp(VisibleRect::center().x, VisibleRect::top().y - 120));
	this->addChild(hintScoreNode);
	hintScoreNode->setVisible(false);
	leftScoreHint = CCLabelBMFont::create("", "shadow_font.fnt");
	leftScoreHint->setPosition(ccp(VisibleRect::center().x, VisibleRect::center().y));
	this->addChild(leftScoreHint);
	leftScoreHint->setVisible(false);

	s_bg = CCLayerColor::create(ccc4(0,0,0,204));
	s_bg->setVisible(false);
	this->addChild(s_bg);
	s_hintSprite = CCSprite::create("start_score_hint.png");
	s_hintSprite->setPosition(ccp(VisibleRect::center().x, 0-VisibleRect::top().y));
	this->addChild(s_hintSprite);

	s_levelLabel = CCLabelBMFont::create("0", "level_font.fnt");
	s_levelLabel->setPosition(ccp(VisibleRect::center().x, s_hintSprite->getContentSize().height/2 + 70));
	s_hintSprite->addChild(s_levelLabel);

	s_targetScore = CCLabelBMFont::create("0", "score_font.fnt");
	s_targetScore->setPosition(ccp(VisibleRect::center().x, s_hintSprite->getContentSize().height/2 - 80));
	s_hintSprite->addChild(s_targetScore);

	CCSprite* pauseNormal = CCSprite::create("pause_button.png");
	CCSprite* pauseSelected = CCSprite::create("pause_button.png");
	pauseNormal->setPosition(ccp(pauseNormal->getContentSize().width*0.1, pauseNormal->getContentSize().height*0.1));
	pauseSelected->setScale(1.2);
	pPauseItem = CCMenuItemSprite::create(pauseNormal, pauseSelected, this, menu_selector(GameLayer::pauseCallback));

	CCMenu* pMenu = CCMenu::create(pPauseItem, NULL);
	pMenu->setPosition(CCPointZero);

	pPauseItem->setPosition(ccp(592, 41));
	topNode->addChild(pMenu);

	topNode->setVisible(false);
	CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic("Cat_BGM.mp3", true);
	return true;
}

void GameLayer::initLevelState()
{
	if(level == 1)
	{
		hasBreak = false;

		//TODO 最高分刷新
		highestScore = userDefault->getIntegerForKey(SCORE_KEY);
		string highestStr = WStrToUTF8(L"最高分 ");
		ostringstream tos;
		tos<<highestScore;
		highestStr = highestStr + tos.str();
		//highestScoreNode->setString(highestStr.c_str());
	}

	ostringstream os;
	os<<level;
	levelNode->setString(os.str().c_str());

	os.str("");
	os<<level;
	s_levelLabel->setString(os.str().c_str());


	os.str("");
	os<<targetScore;
	s_targetScore->setString(os.str().c_str());

	targetScoreNode->setString(os.str().c_str());

	os.str("");
	os<<currentScore;
	currentScoreNode->setString(os.str().c_str());

	//更新进度条
	progressBar->setPercentage(currentScore*100*PERCENTAGE/targetScore);
	progressEndOff->setVisible(true);
	progressEndOn->setVisible(false);
	spark->setVisible(false);
	int tempP;
	if(progressBar->getPercentage() < PERCENTAGE*100)
	{
		tempP = progressBar->getContentSize().width*progressBar->getPercentage()/100;
	}else
	{
		tempP = progressBar->getContentSize().width*PERCENTAGE;
	}
	currentScoreNode->setPosition(ccp(183+tempP, 82-currentScoreNode->getContentSize().height/2));
	

	CCActionInterval* moveIn = CCMoveTo::create(0.5f, ccp(VisibleRect::center().x, VisibleRect::center().y));
	CCActionInterval* moveOut = CCMoveTo::create(0.5f, ccp(VisibleRect::center().x, 0- VisibleRect::top().y));
	CCActionInterval* fadeIn = CCFadeTo::create(0.5f, 204);
	CCActionInterval* fadeOut = CCFadeTo::create(0.5f, 0);


	CCDelayTime *delay_03 = CCDelayTime::create(0.3f);
	CCDelayTime *delay = CCDelayTime::create(1.5f);
	
	CCSequence* sq = CCSequence::create(delay_03, moveIn, delay, moveOut, CCCallFunc::create(this, callfunc_selector(GameLayer::initData)), NULL);
	CCSequence* sq2 = CCSequence::create(fadeIn, CCCA(delay), CCCA(delay_03), fadeOut, NULL);

	s_hintSprite->runAction(sq);
	s_bg->setVisible(true);
	s_bg->setOpacity(0);
	s_bg->runAction(sq2);

	this->schedule(schedule_selector(GameLayer::initFinished), 4.0f);
}

void GameLayer::initData()
{
	s_bg->setVisible(false);
	m_content = new CCArray*[boxSize];
	CCNode* parent = getChildByTag(kNodeTag);
	parent->removeAllChildren();

	cc_timeval psv;
	CCTime::gettimeofdayCocos2d(&psv,NULL);
	unsigned long int seed = psv.tv_sec*1000 + psv.tv_usec/1000;
	srand(seed);

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
	this->setKeypadEnabled(true);
	this->unschedule(schedule_selector(GameLayer::initFinished));
	pPauseItem->setVisible(true);
	topNode->setPosition(ccp(0, VisibleRect::top().y));
	topNode->setVisible(true);
	CCActionInterval* move = CCMoveBy::create(0.3f, ccp(0, -topNode->getContentSize().height));
	topNode->runAction(move);
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
				int temp_count = 0;
				CCARRAY_FOREACH(m_selected, obj)
				{
					Cat* tempCat = (Cat*)obj;
					m_content[tempCat->x]->removeObject(tempCat);
					tempCat->setStatus(2);
					tempCat->explodeQueue(temp_count);
					tempCat->effectDisplay(m_selected->count());
					temp_count++;
				}
				if(m_selected->count()>=10)
				{
					perfectDisplay();
				}
				updateContent();
				updateScore();
				this->setTouchEnabled(false);
				CCCallFunc* update_queue = CCCallFunc::create(this, callfunc_selector(GameLayer::updateQueue));
				CCDelayTime* delay = CCDelayTime::create(explosion_interval*temp_count);
				CCSequence* temp_sq = CCSequence::create(delay, update_queue, NULL);
				this->runAction(temp_sq);
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
				CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("selected.mp3");
				int n = m_selected->count();
				char string[50] = {0};
				sprintf(string, "%d cats %d points", n, getScoreByNum(n));
				hintScoreNode->setString(string);
				hintScoreNode->setScale(0.5f);
				CCActionInterval* scale = CCScaleTo::create(0.4f, 1.0f);
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

void GameLayer::keyBackClicked()
{
	pauseCallback(this);
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

void GameLayer::updateScore()
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

	//增长进度条
	int score_percent = currentScore*100*PERCENTAGE/targetScore;
	if(score_percent > 100)
	{
		score_percent = 100;
	}
	if(progressBar->getPercentage() < 100)
	{
		CCProgressTo* action = CCProgressTo::create(1, score_percent);
		progressBar->runAction(action);
	}
	
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
	if(m_content == NULL)
	{
		return;
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
	this->setTouchEnabled(true);
}

void GameLayer::updateQueue()
{
	updateViewByContent();
	m_selected->release();
	m_selected = NULL;
	hintScoreNode->setVisible(false);
	if(isGameEnd())
	{
		CCDelayTime* delay = CCDelayTime::create(1.8);
		CCCallFunc* processEndFunc = CCCallFunc::create(this, callfunc_selector(GameLayer::processGameEnd));
		CCSequence* sequence = CCSequence::create(delay, processEndFunc, NULL);
		sequence->setTag(SQ_TAG);
		this->stopActionByTag(SQ_TAG);
		this->runAction(sequence);
		pPauseItem->setVisible(false);
		this->setKeypadEnabled(false);
	}
}

/*通过个数算得分*/
int GameLayer::getScoreByNum(int num)
{
	return 5 * num*num;
}

/*剩下个数得分函数*/
int GameLayer::getScoreByLeftNum(int leftNum)
{
	if(leftNum > 10)
	{
		return 0;
	}else
	{
		return 2000 - leftNum*leftNum*20;
	}
}

void GameLayer::stepForScore(float dt)
{
	int score = justScore + (currentScore - justScore) * m_times/60;
	char string[12] = {0};
	sprintf(string, "%d", score);
	currentScoreNode->setString(string);

	//顶部进度条刷新
	if(score >= targetScore)
	{
		progressEndOff->setVisible(false);
		progressEndOn->setVisible(true);
	}
	if(progressBar->getPercentage()<100)
	{
		spark->setVisible(true);
	}else
	{
		spark->setVisible(false);
	}
	int tempP = progressBar->getContentSize().width*progressBar->getPercentage()/100;
	spark->setPosition(ccp(183+tempP, 29));
	if(tempP > progressBar->getContentSize().width*PERCENTAGE)
	{
		tempP = progressBar->getContentSize().width*PERCENTAGE;
	}
	currentScoreNode->setPosition(ccp(183+tempP, 82-currentScoreNode->getContentSize().height/2));

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
	int leftCatNum = 0;
	for(int i=0; i<boxSize; i++)
	{
		CCObject* pObj;
		CCARRAY_FOREACH(m_content[i], pObj)
		{
			Cat* tempCat = (Cat*)pObj;
			tempCat->explodeQueue(leftCatNum);
			leftCatNum++;
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
	m_content = NULL;

	if(leftCatNum < 10)
	{
		justScore = currentScore;
		currentScore = currentScore + getScoreByLeftNum(leftCatNum);
		updateScore();
	}
}

void GameLayer::processLeftCat()
{
	int leftCatNum = 0;
	for(int i=0; i<boxSize; i++)
	{
		CCObject* pObj;
		CCARRAY_FOREACH(m_content[i], pObj)
		{
			Cat* tempCat = (Cat*)pObj;
			tempCat->setStatus(3);
			leftCatNum++;
		}
	}

	//根据剩下猫的个数提示分数
	if(leftCatNum < 10)
	{
		
		char string[50] = {0};
		sprintf(string, "left %d bounds %d", leftCatNum, getScoreByLeftNum(leftCatNum));
		leftScoreHint->setString(string);
		leftScoreHint->setScale(0.5f);
		CCActionInterval* scale = CCScaleTo::create(0.4f, 1.0f);
		CCActionInterval* scale_ease_out = CCEaseElasticOut::create(((CCActionInterval*)scale->copy()->autorelease()));
		CCFadeOut* fadeOut = CCFadeOut::create(0.4f);
		CCDelayTime* delay = CCDelayTime::create(2.0f);
		leftScoreHint->setOpacity(255);
		leftScoreHint->setVisible(true);
		CCSequence* sequence = CCSequence::create(scale_ease_out, delay, fadeOut, NULL);
		leftScoreHint->runAction(sequence);
	}
}

void GameLayer::processGameEnd()
{
	
	this->setTouchEnabled(false);

	CCDelayTime *delay = CCDelayTime::create(1.5f);
	CCDelayTime *delay1 = CCDelayTime::create(2.5f);
	CCCallFunc* processCat = CCCallFunc::create(this, callfunc_selector(GameLayer::processLeftCat));
	CCCallFunc* removeCat = CCCallFunc::create(this, callfunc_selector(GameLayer::removeLeftCat));

	CCCallFunc* gameEnd = CCCallFunc::create(this, callfunc_selector(GameLayer::gameEndCallback));

	CCSequence* sq = CCSequence::create(processCat, delay, removeCat, delay1, gameEnd, NULL);
	this->runAction(sq);

	CCDelayTime* delay2 = CCDelayTime::create(3.0f);
	CCActionInterval* moveout = CCMoveBy::create(0.3f, ccp(0, topNode->getContentSize().height));
	CCSequence* sq2 = CCSequence::create(delay2, moveout, NULL);
	topNode->runAction(sq2);
}

void GameLayer::gameEndCallback()
{
	//记录最高分
	if(currentScore > highestScore)
	{
		hasBreak = true;
		userDefault->setIntegerForKey(SCORE_KEY, currentScore);
	}

	if(currentScore >= targetScore)
	{
		level++;
		if(level > 12)
		{
			targetScore = levelScore[11] + (level-12)*2500;
		}else
		{
			targetScore = levelScore[level-1];
		}
		//result_label->setString("win");
	}else
	{
		level = 1;
		targetScore = levelScore[level-1];
		result_score = currentScore;
		currentScore = 0;
		justScore = 0;
		//提交分数
		#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
			JniMethodInfo minfo;
			bool isHave = JniHelper::getStaticMethodInfo(minfo, "com/taobao/popcat/popCat", "commitGameScore", "(I)V");
			if(isHave)
			{
				minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID, result_score);
			}
		#elif(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
			//TODO
		#endif
	}

	if(level > 1)
	{
		GameEndLayer* endlayer = GameEndLayer::create(1);
		this->addChild(endlayer);
	}else
	{
		GameEndLayer* endlayer;
		if(hasBreak)
		{
			endlayer = GameEndLayer::create(2);
		}else
		{
			endlayer = GameEndLayer::create(0);
		}
		this->addChild(endlayer);
	}
}

void GameLayer::pauseCallback(CCObject* pSender)
{
	if(!isPause)
	{
		PauseLayer* pauselayer = PauseLayer::create();
		pauselayer->initWithColor(ccc4(0,0,0,204));
		CCNode* node = this->getParent();
		node->addChild(pauselayer);
		CocosDenshion::SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
		isPause = true;

		/*int random = (int)(CCRANDOM_0_1() * 100)%3;
		GameEndLayer* endlayer = GameEndLayer::create(random);
		this->addChild(endlayer);
		isPause = true;*/
	}
}

void GameLayer::perfectDisplay()
{
	CCSprite* bg = CCSprite::create("perfect_run_bg.png");
	bg->setPosition(ccp(VisibleRect::center().x, VisibleRect::top().y-150));
	this->addChild(bg);
	for(int i=0; i<5;i++)
	{
		CCSprite* temp = CCSprite::create("perfect_run.png");
		float scale;
		int actualY;
		if(i%2>0)
		{
			scale = 1.0f;
			switch (i)
			{
			case 1:
				actualY = 50;
				break;
			case 3:
				actualY = 42;
				break;
			default:
				actualY = 40;
				break;
			}
		}else
		{
			switch (i)
			{
			case 0:
				scale = 0.6f;
				actualY = 77;
				break;
			case 2:
				scale = 0.8f;
				actualY = 90;
				break;
			case 4:
				scale = 0.7f;
				actualY = 85;
				break;
			default:
				scale = 0.8f;
				actualY = 90;
				break;
			}
			
		}
		temp->setScale(scale);
		temp->setPosition(ccp(VisibleRect::right().x+84, actualY));
		bg->addChild(temp);
		CCMoveTo* move = CCMoveTo::create(0.8f, ccp(0-84, actualY));
		CCDelayTime* delay = CCDelayTime::create(0.127*i);
		CCFiniteTimeAction* runFinished = CCCallFuncN::create(this, callfuncN_selector(GameLayer::perfectRunFinished));
		temp->runAction(CCSequence::create(delay, move, runFinished, NULL));
	}
	CCFiniteTimeAction* finished = CCCallFuncN::create(this, callfuncN_selector(GameLayer::perfectRunFinished));
	CCDelayTime* delay2 = CCDelayTime::create(1.2);
	CCFadeOut* fadeout = CCFadeOut::create(0.3);
	bg->runAction(CCSequence::create(delay2, fadeout, finished, NULL));
}

void GameLayer::perfectRunFinished(CCNode* sender)
{
	CCSprite* sprite = (CCSprite*)sender;
	sprite->removeFromParent();
}

bool PauseLayer::init()
{
	if(!CCLayerColor::init())
	{
		return false;
	}

	CCSprite* goNormal = CCSprite::create("start_button.png");
	CCSprite* goSelected = CCSprite::create("start_button.png");
	goSelected->setScale(1.2);
	goNormal->setPosition(ccp(goNormal->getContentSize().width * 0.1, goNormal->getContentSize().height * 0.1));
	CCMenuItemSprite* goMenu = CCMenuItemSprite::create(goNormal, goSelected, this, menu_selector(PauseLayer::goOnCallback));

	CCSprite* homeNormal = CCSprite::create("home_button.png");
	CCSprite* homeSelected = CCSprite::create("home_button.png");
	homeSelected->setScale(1.2);
	homeNormal->setPosition(ccp(homeNormal->getContentSize().width * 0.1, homeNormal->getContentSize().height * 0.1));
	CCMenuItemSprite* homeMenu = CCMenuItemSprite::create(homeNormal, homeSelected, this, menu_selector(PauseLayer::homeCallback));

	CCMenu* menu = CCMenu::create(goMenu, homeMenu, NULL);
	menu->setPosition(CCPointZero);
	goMenu->setPosition(ccp(VisibleRect::center().x, VisibleRect::center().y- 50));
	homeMenu->setPosition(ccp(450, 240));
	this->addChild(menu);
	this->setTouchEnabled(true);
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);

	goMenu->setScale(0);
	homeMenu->setScale(0);
	CCScaleTo* scale = CCScaleTo::create(1.2f, 1.0);
	CCActionInterval* scale_out1 = CCEaseElasticOut::create(((CCActionInterval*)scale->copy()->autorelease()), 0.3);
	CCActionInterval* scale_out2 = CCEaseElasticOut::create(((CCActionInterval*)scale->copy()->autorelease()), 0.3);
	CCDelayTime *delay = CCDelayTime::create(0.2f);

	CCSequence* sq2 = CCSequence::create(delay, scale_out2, NULL);
	goMenu->runAction(scale_out1);
	homeMenu->runAction(sq2);

	return true;
}

void PauseLayer::goOnCallback(CCObject* pSender)
{
	CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
	this->removeFromParentAndCleanup(true);
	CocosDenshion::SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
	isPause = false;
}

void PauseLayer::homeCallback(CCObject* pSender)
{
	CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
	CCScene* scene = GameMenu::createScene();
	CCTransitionScene* pScene = CCTransitionCrossFade::create(0.5f, scene);
	CCDirector::sharedDirector()->replaceScene(pScene);
	isPause = false;
}

bool PauseLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
	return true;
}

GameEndLayer::GameEndLayer()
{
	layer_state = 0;
}

GameEndLayer* GameEndLayer::create(int s)
{
	GameEndLayer* layer = new GameEndLayer();
	layer->setLayerState(s);
	if(layer && layer->init())
	{
		layer->autorelease();
		return layer;
	}else
	{
		CC_SAFE_DELETE(layer);
		return NULL;
	}
}

void GameEndLayer::setLayerState(int state)
{
	layer_state = state;
}

bool GameEndLayer::init()
{
	if(!CCLayer::init())
	{
		return false;
	}
	string bg_file, disk_file, hint_file;
	switch (layer_state)
	{
	case 0:
		bg_file = "game_over_bg.png";
		disk_file = "game_over_disk.png";
		hint_file = "game_over_hint.png";
		break;
	case 1:
		bg_file = "next_level_bg.png";
		disk_file = "next_level_disk.png";
		hint_file = "next_level_hint.png";
		break;
	case 2:
		bg_file = "new_record_bg.png";
		disk_file = "new_record_disk.png";
		hint_file = "new_record_hint.png";
		break;
	default:
		bg_file = "game_over_bg.png";
		disk_file = "game_over_disk.png";
		hint_file = "game_over_hint.png";
		break;
	}

	CCSprite* bg = CCSprite::create(bg_file.c_str());
	bg->setPosition(VisibleRect::center());
	this->addChild(bg);

	CCSprite* hint = CCSprite::create(hint_file.c_str());
	hint->setPosition(ccp(VisibleRect::center().x, 430));
	this->addChild(hint);

	CCSprite* disk = CCSprite::create(disk_file.c_str());
	disk->setPosition(ccp(VisibleRect::center().x, 569));
	this->addChild(disk);

	hint->setScale(0);
	disk->setScale(0);
	CCActionInterval* scale = CCScaleTo::create(1.2f, 1.0);
	CCActionInterval* scale_out1 = CCEaseElasticOut::create(((CCActionInterval*)scale->copy()->autorelease()), 0.3);
	CCActionInterval* scale_out2 = CCEaseElasticOut::create(((CCActionInterval*)scale->copy()->autorelease()), 0.3);
	hint->runAction(scale_out1);
	disk->runAction(scale_out2);
	if(layer_state == 1)
	{
		CCSprite* goNormal = CCSprite::create("start_button.png");
		CCSprite* goSelected = CCSprite::create("start_button.png");
		goSelected->setScale(1.2);
		goNormal->setPosition(ccp(goNormal->getContentSize().width*0.1, goNormal->getContentSize().height*0.1));
		CCMenuItemSprite* goMenu = CCMenuItemSprite::create(goNormal, goSelected, this, menu_selector(GameEndLayer::goOnCallback));

		CCMenu* menu = CCMenu::create(goMenu, NULL);
		menu->setPosition(CCPointZero);
		goMenu->setPosition(ccp(VisibleRect::center().x, 200));
		this->addChild(menu);

		goMenu->setScale(0);
		CCActionInterval* scale_out3 = CCEaseElasticOut::create(((CCActionInterval*)scale->copy()->autorelease()), 0.3);
		CCDelayTime* delay = CCDelayTime::create(0.2);
		CCSequence* sq = CCSequence::create(delay, scale_out3, NULL);
		goMenu->runAction(sq);
	}else
	{
		//显示分数
		CCLabelBMFont* scoreLabel = CCLabelBMFont::create("0", "shadow_font.fnt");
		char string[12] = {0};
		sprintf(string, "%d", result_score);
		scoreLabel->setString(string);
		scoreLabel->setPosition(ccp(VisibleRect::center().x, 795 + scoreLabel->getContentSize().height/2));
		this->addChild(scoreLabel);
		scoreLabel->setScale(0);
		CCActionInterval* scaleForLabel = CCScaleTo::create(1.2f, 1.5);
		CCActionInterval* scale_label = CCEaseElasticOut::create(((CCActionInterval*)scaleForLabel->copy()->autorelease()), 0.3);
		scoreLabel->runAction(scale_label);

		CCSprite* goNormal = CCSprite::create("restart_button.png");
		CCSprite* goSelected = CCSprite::create("restart_button.png");
		goSelected->setScale(1.2);
		goNormal->setPosition(ccp(goNormal->getContentSize().width*0.1, goNormal->getContentSize().height*0.1));
		CCMenuItemSprite* goMenu = CCMenuItemSprite::create(goNormal, goSelected, this, menu_selector(GameEndLayer::goOnCallback));

		CCSprite* homeNormal = CCSprite::create("home_button.png");
		CCSprite* homeSelected = CCSprite::create("home_button.png");
		homeSelected->setScale(1.2);
		homeNormal->setPosition(ccp(homeNormal->getContentSize().width*0.1, homeNormal->getContentSize().height*0.1));
		CCMenuItemSprite* homeMenu = CCMenuItemSprite::create(homeNormal, homeSelected, this, menu_selector(GameEndLayer::homeCallback));

		CCMenu* menu = CCMenu::create(goMenu, homeMenu, NULL);
		menu->setPosition(CCPointZero);
		goMenu->setPosition(ccp(VisibleRect::center().x, 200));
		homeMenu->setPosition(ccp(499, 94));
		this->addChild(menu);

		goMenu->setScale(0);
		homeMenu->setScale(0);
		CCActionInterval* scale_out3 = CCEaseElasticOut::create(((CCActionInterval*)scale->copy()->autorelease()), 0.3);
		CCActionInterval* scale_out4 = CCEaseElasticOut::create(((CCActionInterval*)scale->copy()->autorelease()), 0.3);
		CCDelayTime* delay3 = CCDelayTime::create(0.2);
		CCDelayTime* delay4 = CCDelayTime::create(0.4);
		CCSequence* sq3 = CCSequence::create(delay3, scale_out3, NULL);
		CCSequence* sq4 = CCSequence::create(delay4, scale_out4, NULL);
		goMenu->runAction(sq3);
		homeMenu->runAction(sq4);
	}

	this->setTouchEnabled(true);
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);

	return true;
}

void GameEndLayer::goOnCallback(CCObject* pSender)
{
	CCNode* node = this->getParent();
	CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
	this->removeFromParentAndCleanup(true);
	CCDelayTime* delay = CCDelayTime::create(0.2);
	CCCallFunc* initLevel = CCCallFunc::create(node, callfunc_selector(GameLayer::initLevelState));
	CCSequence* sq = CCSequence::create(delay, initLevel, NULL);
	node->runAction(sq);
	isPause = false;
}

void GameEndLayer::homeCallback(CCObject* pSender)
{
	CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
	CCScene* scene = GameMenu::createScene();
	CCTransitionScene* pScene = CCTransitionCrossFade::create(0.5f, scene);
	CCDirector::sharedDirector()->replaceScene(pScene);
	isPause = false;
}

bool GameEndLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
	return true;
}