#ifndef __CAT_H_
#define __CAT_H_
#include "cocos2d.h"

enum
{
	RedCat = 0,
	BuleCat,
	GreenCat,
	OrangeCat,
	PurpleCat,
	YellowCat,
};
static const char* myCat[] = {"cai_red.png", "cat_blue.png", "cat_green.png", "cat_orange.png", "cat_purple.png", "cat_yellow.png"};
class Cat : public cocos2d::CCSprite
{
public:
	static Cat* CreateWithNum(int num);

	void setXandY(int ax, int ay);

	virtual CCObject* copyWithZone(cocos2d::CCZone *pZone);

	virtual void draw(void);

	int catColor;
	int status; //0 default 1 selected
	int x;
	int y;
};
#endif // !__CAT_H_
