#include "Cat.h"
#include "draw_nodes/CCDrawingPrimitives.h"

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
