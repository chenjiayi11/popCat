#ifndef __MENUSCENE_H_
#define __MENUSCENE_H_
#include "cocos2d.h"
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "platform\android\jni\JniHelper.h"
#endif
USING_NS_CC;

class GameMenu : public CCLayer
{
public:
	virtual bool init();

	virtual void onEnterTransitionDidFinish();

	virtual void keyBackClicked();

	static CCScene* createScene();

	void menuStartCallback(CCObject* pSender);

	void menuPaiHangCallback(CCObject* pSender);

	void enterGuide();

	void skipCallback(CCObject* pSender);

	CREATE_FUNC(GameMenu);

	#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
		void showList(JNIEnv* env, jobjectArray array);
	#endif
private:
	CCSprite* topSprite;
	CCMenuItemSprite* menuStart;
	CCMenuItemSprite* menuPH;
	bool isGuiding;

};
#endif
