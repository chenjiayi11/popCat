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
	int highestScore; //最高分
	int currentScore; //当前分
	int targetScore;  //目标分
	int level;		  //关卡
};

#endif