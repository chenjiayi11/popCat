#include "MenuScene.h"
#include "VisibleRect.h"
#include "GameScene.h"


CCScene* GameMenu::createScene()
{
	CCScene* scene = CCScene::create();

	GameMenu* layer = GameMenu::create();

	scene->addChild(layer);

	return scene;
}

bool GameMenu::init()
{
	if(!CCLayer::init())
	{
		return false;
	}

	CCSprite* menu_bg = CCSprite::create("menu_bg.png");
	menu_bg->setPosition(VisibleRect::center());

	this->addChild(menu_bg, -1);

	topSprite = CCSprite::create("menu_title.png");
	topSprite->setPosition(ccp(menu_bg->getContentSize().width/2, menu_bg->getContentSize().height-topSprite->getContentSize().height/2 - 20));
	menu_bg->addChild(topSprite);
	topSprite->setScale(0);
	topSprite->setVisible(false);


	CCSprite* startNormal = CCSprite::create("start_button.png");
	CCSprite* startSelected = CCSprite::create("start_button.png");
	startNormal->setPosition(ccp(startNormal->getContentSize().width*0.1, startNormal->getContentSize().height*0.1));
	startSelected->setScale(1.2);
	menuStart = CCMenuItemSprite::create(startNormal, startSelected, this, menu_selector(GameMenu::menuStartCallback));

	CCSprite* phNormal = CCSprite::create("paiming.png");
	CCSprite* phSelected = CCSprite::create("paiming.png");
	phNormal->setPosition(ccp(phNormal->getContentSize().width*0.1, phNormal->getContentSize().height*0.1));
	phSelected->setScale(1.2);
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

	return true;
}

void GameMenu::onEnterTransitionDidFinish()
{
	topSprite->setVisible(true);
	menuStart->setVisible(true);
	menuPH->setVisible(true);
	CCScaleTo* scale = CCScaleTo::create(2.0f, 1.0);
	CCActionInterval* scale_out = CCEaseElasticOut::create(((CCActionInterval*)scale->copy()->autorelease()), 0.4);
	CCActionInterval* scale_out1 = CCEaseElasticOut::create(((CCActionInterval*)scale->copy()->autorelease()), 0.3);
	CCActionInterval* scale_out2 = CCEaseElasticOut::create(((CCActionInterval*)scale->copy()->autorelease()), 0.3);
	CCDelayTime *delay = CCDelayTime::create(0.2f);
	CCDelayTime *delay2 = CCDelayTime::create(0.4f);

	CCSequence* sq1 = CCSequence::create(delay, scale_out1, NULL);
	CCSequence* sq2 = CCSequence::create(delay2, scale_out2, NULL);
	topSprite->runAction(scale_out);
	menuStart->runAction(sq1);
	menuPH->runAction(sq2);
}

void GameMenu::menuStartCallback(CCObject* pSender)
{
	CCScene* scene = GameLayer::createScene();
	CCTransitionScene* tScene = CCTransitionTurnOffTiles::create(0.5f, scene);
	CCDirector::sharedDirector()->replaceScene(tScene);
	
}

void GameMenu::menuPaiHangCallback(CCObject* pSender)
{
}
