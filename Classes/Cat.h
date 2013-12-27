#ifndef __CAT_H_
#define __CAT_H_
#include "cocos2d.h"
USING_NS_CC;
enum
{
	RedCat = 0,
	BuleCat,
	GreenCat,
	PurpleCat,
	YellowCat,
};
static const char* myCat[] = {"cat_red_%d.png", "cat_blue_%d.png", "cat_green_%d.png", "cat_purple_%d.png", "cat_yellow_%d.png"};
static CCSpriteFrameCache* cache = CCSpriteFrameCache::sharedSpriteFrameCache();
class Cat : public CCSprite
{
public:
	static Cat* CreateWithNum(int num);
	static void initCache();

	void setXandY(int ax, int ay);

	virtual CCObject* copyWithZone(cocos2d::CCZone *pZone);

	virtual void setToSelected(bool b);
	virtual void setStatus(int s);

	int catColor;
	int x;
	int y;
private:
	int status; //0 default 1 selected 2 would be delete
};
#endif // !__CAT_H_
