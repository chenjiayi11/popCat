#ifndef __GAMESCENE_H__
#define __GAMESCENE_H__
#include "cocos2d.h"
#include "Cat.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;
using namespace std;

#define CCCA(x)   (x->copy()->autorelease())
#define userDefault CCUserDefault::sharedUserDefault()

class GameLayer : public CCLayer
{
public:
	virtual bool init();
	GameLayer();
	~GameLayer();

	static CCScene* createScene();

	virtual void ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent);

	virtual void ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent);

	virtual void ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent);

	void initLevelState();

	void initData();

	void initFinished(float dt);

	void popCat(CCObject* object, CCArray* array);

	void WStrToUTF8(string& dest, const wstring& src);

	string WStrToUTF8(const wstring& str);

	void playParticleEffect(CCPoint point);

	void updateContent();

	void updateViewByContent();

	void stepForScore(float dt);

	int getScoreByNum(int num);

	int getRandomColor();

	bool isGameEnd();

	void removeLeftCat();
	void processLeftCat();
	void processGameEnd();

	CREATE_FUNC(GameLayer);

private:

	CCLabelTTF* highestScoreNode;
	CCLabelTTF* targetScoreNode;
	CCLabelTTF* levelNode;
	CCLabelTTF* currentScoreNode;
	CCLabelTTF* hintScoreNode;

	CCLabelTTF* s_levelLabel;
	CCLabelTTF* s_label;
	CCLabelTTF* s_targetScore;

	CCLabelBMFont* result_label;

	int highestScore; //最高分
	int currentScore; //当前分
	int targetScore;  //目标分
	int level;		  //关卡
	int m_times;
	int justScore;

	//CCArray* m_content;
	CCArray* m_selected;
	//Cat*** m_content;
	CCArray** m_content;
};

#endif
