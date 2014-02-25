#include "Cat.h"
#include "AppMacros.h"

USING_NS_CC;

Cat* Cat::CreateWithNum(int num)
{
	Cat* cat = new Cat();
	cat->catColor = num;
	cat->status = 0;
	char string[20] = {0};
	sprintf(string, myCat[num], 1);
	if(cat->initWithSpriteFrame(cache->spriteFrameByName(string)))
	{
		cat->autorelease();
		return cat;
	}
	CC_SAFE_DELETE(cat);
	return NULL;
}

void Cat::initCache()
{
	cache->addSpriteFramesWithFile("my_cats.plist");
	CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("explosion_new.mp3");
}

void Cat::setXandY(int ax, int ay)
{
	this->x = ax;
	this->y = ay;
}

CCObject* Cat::copyWithZone(CCZone *pZone)
{
	this->retain();
	return this;
}

void Cat::setToSelected(bool b)
{
	char string[20] = {0};
	if(b)
	{
		status = 1;
		sprintf(string, myCat[catColor], 2);
		this->setDisplayFrame(cache->spriteFrameByName(string));
	}else
	{
		status = 0;
		sprintf(string, myCat[catColor], 1);
		this->setDisplayFrame(cache->spriteFrameByName(string));
	}
	

}

void Cat::setStatus(int s)
{
	status = s;
	switch (s)
	{
	default:
		setToSelected(false);
		break;
	case 0:
		setToSelected(false);
		break;
	case 1:
		setToSelected(true);
		break;
	case 3:
		char string[20] = {0};
		sprintf(string, myCat[catColor], 3);
		this->setDisplayFrame(cache->spriteFrameByName(string));
		break;
	}
}

void Cat::playParticleEffect()
{
	CCParticleSystemQuad *particle = CCParticleExplosion::create();
	particle->setTexture(CCTextureCache::sharedTextureCache()->addImage("fire.png"));
	particle->setDuration(0.1f);
	particle->setGravity(ccp(0,-100));
	particle->setLifeVar(0);
	particle->setLife(1.0f);
	particle->setSpeed(100);
	particle->setSpeedVar(40);
	particle->setEmissionRate(1000);
	particle->setTotalParticles(100);
	particle->setAutoRemoveOnFinish(true);
	particle->setStartColor(p_colors[catColor]);
	particle->setStartColorVar(ccc4f(0.2,0.2,0.2,0));
	particle->setEndColorVar(ccc4f(0.2,0.2,0.2,0));
	CCNode* node = this->getParent()->getParent();
	node->addChild(particle);
	particle->setPosition(ccp(x*catSize + catSize/2,y*catSize + catSize/2));
	CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("explosion_new.mp3");
	this->removeFromParentAndCleanup(true);
}

void Cat::explodeQueue(int n)
{
	if(n > 15)
	{
		n = 15;
	}
	CCDelayTime *delay = CCDelayTime::create(explosion_interval*n);
	CCCallFunc* playEffect = CCCallFunc::create(this, callfunc_selector(Cat::playParticleEffect));
	CCSequence* sq = CCSequence::create(delay, playEffect, NULL);
	this->runAction(sq);
}
