#ifndef __GAMESCENE_H__
#define __GAMESCENE_H__
#include "cocos2d.h"

#define userDefault cocos2d::CCUserDefault::sharedUserDefault()

class GameLayer : public cocos2d::CCLayer
{
public:
	virtual bool init();

	static cocos2d::CCScene* createScene();

	CREATE_FUNC(GameLayer);

private:
	int highestScore; //��߷�
	int currentScore; //��ǰ��
	int targetScore;  //Ŀ���
	int level;		  //�ؿ�
};

#endif