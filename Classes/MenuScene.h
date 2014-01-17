#ifndef __MENUSCENE_H_
#define __MENUSCENE_H_
#include "cocos2d.h"

USING_NS_CC;

class GameMenu : public CCLayer
{
public:
	virtual bool init();

	virtual void onEnterTransitionDidFinish();

	static CCScene* createScene();

	void menuStartCallback(CCObject* pSender);

	void menuPaiHangCallback(CCObject* pSender);

	CREATE_FUNC(GameMenu);

private:
	CCSprite* topSprite;			//上面的文字
	CCMenuItemSprite* menuStart;	//开始按钮
	CCMenuItemSprite* menuPH;		//排行按钮
};
#endif