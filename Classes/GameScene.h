#ifndef __GAMESCENE_H__
#define __GAMESCENE_H__
#include "cocos2d.h"
#include "Cat.h"

USING_NS_CC;
using namespace std;

#define userDefault cocos2d::CCUserDefault::sharedUserDefault()

class GameLayer : public cocos2d::CCLayer
{
public:
	virtual bool init();
	GameLayer();
	~GameLayer();

	static cocos2d::CCScene* createScene();

	virtual void ccTouchesBegan(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);

	virtual void ccTouchesMoved(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);

	virtual void ccTouchesEnded(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);

	void initData();

	void GameLayer::initFinished(float dt);

	void GameLayer::popCat(Cat* cat, CCArray* array);

	void GameLayer::WStrToUTF8(string& dest, const wstring& src);

	string GameLayer::WStrToUTF8(const wstring& str);

	CREATE_FUNC(GameLayer);

private:

	CCLabelTTF* highestScoreNode;
	CCLabelTTF* targetScoreNode;
	CCLabelTTF* levelNode;
	CCLabelTTF* currentScoreNode;

	int highestScore; //最高分
	int currentScore; //当前分
	int targetScore;  //目标分
	int level;		  //关卡

	//CCArray* m_content;
	CCArray* m_selected;
	Cat*** m_content;
};

#endif