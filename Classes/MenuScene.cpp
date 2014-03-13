#include "MenuScene.h"
#include "VisibleRect.h"
#include "GameScene.h"
#include "AppMacros.h"

#define GUIDE_KEY "has_guide"
GameMenu* menuInstance = 0;
CCScene* GameMenu::createScene()
{
	CCScene* scene = CCScene::create();

	GameMenu* layer = GameMenu::create();

	scene->addChild(layer);

	#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	menuInstance = layer;
	#endif

	return scene;
}

bool GameMenu::init()
{
	if(!CCLayer::init())
	{
		return false;
	}
	isGuiding = false;
	CCSprite* menu_bg = CCSprite::create("menu_bg.png");
	float scalY = VisibleRect::top().y/menu_bg->getContentSize().height;
	menu_bg->setScaleY(scalY);
	menu_bg->setPosition(VisibleRect::center());

	this->addChild(menu_bg, -1);

	topSprite = CCSprite::create("menu_title.png");
	topSprite->setPosition(ccp(VisibleRect::center().x, VisibleRect::top().y-topSprite->getContentSize().height/2 - 20));
	this->addChild(topSprite);
	topSprite->setScale(0);
	topSprite->setVisible(false);


	CCSprite* startNormal = CCSprite::create("start_button.png");
	CCSprite* startSelected = CCSprite::create("start_button.png");
	startNormal->setPosition(ccp(startNormal->getContentSize().width*0.1, startNormal->getContentSize().height*0.1));
	startSelected->setScale(1.2f);
	menuStart = CCMenuItemSprite::create(startNormal, startSelected, this, menu_selector(GameMenu::menuStartCallback));

	CCSprite* phNormal = CCSprite::create("paiming.png");
	CCSprite* phSelected = CCSprite::create("paiming.png");
	phNormal->setPosition(ccp(phNormal->getContentSize().width*0.1, phNormal->getContentSize().height*0.1));
	phSelected->setScale(1.2f);
	menuPH = CCMenuItemSprite::create(phNormal, phSelected, this, menu_selector(GameMenu::menuPaiHangCallback));

	CCMenu* menu = CCMenu::create(menuStart, menuPH, NULL);
	menu->setPosition(CCPointZero);
	menuStart->setPosition(ccp(VisibleRect::center().x, VisibleRect::center().y- 50));
	menuPH->setPosition(ccp(450, 240));
	menuStart->setScale(0);
	menuStart->setVisible(false);
	menuPH->setScale(0);
	menuPH->setVisible(false);

	this->addChild(menu);
	this->setKeypadEnabled(true);

	return true;
}

void GameMenu::onEnterTransitionDidFinish()
{
	topSprite->setVisible(true);
	menuStart->setVisible(true);
	menuPH->setVisible(true);
	CCScaleTo* scale = CCScaleTo::create(2.0f, 1.0);
	CCActionInterval* scale_out = CCEaseElasticOut::create(((CCActionInterval*)scale->copy()->autorelease()), 0.4f);
	CCActionInterval* scale_out1 = CCEaseElasticOut::create(((CCActionInterval*)scale->copy()->autorelease()), 0.3f);
	CCActionInterval* scale_out2 = CCEaseElasticOut::create(((CCActionInterval*)scale->copy()->autorelease()), 0.3f);
	CCDelayTime *delay = CCDelayTime::create(0.2f);
	CCDelayTime *delay2 = CCDelayTime::create(0.4f);

	CCSequence* sq1 = CCSequence::create(delay, scale_out1, NULL);
	CCSequence* sq2 = CCSequence::create(delay2, scale_out2, NULL);
	topSprite->runAction(scale_out);
	menuStart->runAction(sq1);
	menuPH->runAction(sq2);
}

void GameMenu::keyBackClicked()
{
	CCDirector::sharedDirector()->end();
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}

void GameMenu::menuStartCallback(CCObject* pSender)
{
	if(isGuiding)
	{
		return;
	}

	if(userDefault->getBoolForKey(GUIDE_KEY, false))
	{
		CCScene* scene = GameLayer::createScene();
		CCTransitionScene* tScene = CCTransitionTurnOffTiles::create(0.5f, scene);
		CCDirector::sharedDirector()->replaceScene(tScene);
	}else
	{
		enterGuide();
	}
	
}

void GameMenu::menuPaiHangCallback(CCObject* pSender)
{
	#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	JniMethodInfo minfo;
	bool isHave = JniHelper::getStaticMethodInfo(minfo, "com/taobao/popcat/popCat", "getScoreList", "()V");
	if(isHave)
	{
		minfo.env->CallStaticObjectMethod(minfo.classID, minfo.methodID);
		CCLOG("showList returns from java");
	}
	#elif(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	#endif
}

#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
void GameMenu::showList(JNIEnv* env, jobjectArray array)
{
		CCLabelTTF* tempL = CCLabelTTF::create("test", "arial", 24);
		tempL->setPosition(ccp(VisibleRect::center().x, VisibleRect::top().y - 50));
		jobject temp_obj = env->GetObjectArrayElement(array, 0);
		JniMethodInfo minfo;
		bool isHave;
		isHave = JniHelper::getMethodInfo(minfo, "com/laiwang/opensdk/model/UserGameInfo", "getValue", "()I");
		jint value = minfo.env->CallIntMethod(temp_obj, minfo.methodID);
		CCLOG("showList returns from java :%d", value);
		isHave = JniHelper::getMethodInfo(minfo, "com/laiwang/opensdk/model/UserGameInfo", "getName", "()Ljava/lang/String;");
		jstring name = (jstring)(minfo.env->CallObjectMethod(temp_obj, minfo.methodID));
		char* namec = (char*)(minfo.env->GetStringUTFChars(name,0));
		CCLOG("showList returns from java :%s", namec);
		tempL->setString(namec);
		this->addChild(tempL);
	
}
#endif

void GameMenu::enterGuide()
{
	isGuiding = true;
	CCSprite* guide_bg = CCSprite::create("guide_bg.png");
	float scaleY = VisibleRect::top().y/guide_bg->getContentSize().height;
	guide_bg->setScaleY(scaleY);
	guide_bg->setPosition(VisibleRect::center());

	CCSprite* guide = CCSprite::create("guide.png");
	guide->setPosition(ccp(VisibleRect::center().x, guide_bg->getContentSize().height/2));
	guide_bg->addChild(guide);

	CCSprite* skip_n = CCSprite::create("guide_skip.png");
	CCSprite* skip_s = CCSprite::create("guide_skip.png");
	skip_n->setPosition(ccp(skip_n->getContentSize().width*0.1, skip_n->getContentSize().height*0.1));
	skip_s->setScale(1.2f);
	CCMenuItemSprite* skipItem = CCMenuItemSprite::create(skip_n, skip_s, this, menu_selector(GameMenu::skipCallback));
	CCMenu* menu = CCMenu::create(skipItem, NULL);
	menu->setPosition(CCPointZero);
	skipItem->setAnchorPoint(ccp(0.5,0.5));
	skipItem->setPosition(ccp(278, 60));
	guide->addChild(menu);
	guide->setCascadeOpacityEnabled(true);

	this->addChild(guide_bg);
	CCFadeIn* fadein = CCFadeIn::create(0.5);
	guide_bg->setCascadeOpacityEnabled(true);
	guide_bg->runAction(fadein);
}

void GameMenu::skipCallback(CCObject* pSender)
{
	userDefault->setBoolForKey(GUIDE_KEY, true);
	CCScene* scene = GameLayer::createScene();
	CCTransitionScene* tScene = CCTransitionTurnOffTiles::create(0.5f, scene);
	CCDirector::sharedDirector()->replaceScene(tScene);
}
