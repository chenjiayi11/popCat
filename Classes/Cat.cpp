#include "Cat.h"
#include "draw_nodes/CCDrawingPrimitives.h"

USING_NS_CC;

Cat* Cat::CreateWithNum(int num)
{
	Cat* cat = new Cat();
	cat->catColor = num;
	cat->status = 0;
	if(cat->initWithFile(myCat[num]))
	{
		cat->autorelease();
		return cat;
	}
	CC_SAFE_DELETE(cat);
	return NULL;
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

void Cat::draw(void)
{
	CCSprite::draw();

	if(status == 1)
	{
		CCPoint p1 = ccp(1,1);
		CCPoint p2 = ccp(this->getContentSize().width-1, 1);
		CCPoint p3 = ccp(this->getContentSize().width-1,this->getContentSize().height-1);
		CCPoint p4 = ccp(1, this->getContentSize().height-1);
		ccDrawColor4B(255,255,255,88);
		glLineWidth(1);
		ccDrawLine(p1, p2);
		ccDrawLine(p2, p3);
		ccDrawLine(p3, p4);
		ccDrawLine(p4, p1);
	}
	
}