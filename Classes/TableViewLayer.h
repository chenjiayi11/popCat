#ifndef __TABLEVIEWLAYER_H_
#define __TABLEVIEWLAYER_H_

#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;
typedef struct MyStruct
{
	int value;
	char* name;
	char* avatar;
}CellItem;

class TableViewLayer : public CCLayer, public CCTableViewDelegate, public CCTableViewDataSource
{
	virtual void tableCellTouched(CCTableView* table, CCTableViewCell* cell);

	virtual CCSize tableCellSizeForIndex(CCTableView* table, unsigned int idx);

	virtual CCTableViewCell* tableCellAtIndex(CCTableView* table, unsigned int idx);

	virtual unsigned int numberOfCellsInTableView(CCTableView* table);

	virtual void scrollViewDidScroll(CCScrollView* view) {}

	virtual void scrollViewDidZoom(CCScrollView* view) {}
};
#endif