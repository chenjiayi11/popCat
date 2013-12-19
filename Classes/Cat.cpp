#include "Cat.h"
#include "draw_nodes/CCDrawingPrimitives.h"

Cat* Cat::CreateWithNum(int num)
{
	Cat* cat = new Cat();
	cat->catColor = num;
	if(cat->initWithFile(myCat[num]))
	{
		cat->autorelease();
		return cat;
	}
	CC_SAFE_DELETE(cat);
	return NULL;
}

void Cat::draw(void)
{
	CCSprite::draw();
	glEnable(GL_LINE_SMOOTH);
}