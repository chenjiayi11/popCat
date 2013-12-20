#ifndef __GAMESCENE_H__
#define __GAMESCENE_H__
#include "cocos2d.h"
#include "Cat.h"

USING_NS_CC;

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

	void GameLayer::popCat(Cat* cat, CCArray* array);

	CREATE_FUNC(GameLayer);

private:
	int highestScore; //��߷�
	int currentScore; //��ǰ��
	int targetScore;  //Ŀ���
	int level;		  //�ؿ�

	//CCArray* m_content;
	CCArray* m_selected;
	Cat*** m_content;
};

#endif