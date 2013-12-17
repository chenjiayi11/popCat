#ifndef __APPMACROS_H__
#define __APPMACROS_H__

#include "cocos2d.h"

USING_NS_CC;

#define DESIGN_RESOLUTION_320x480 0
#define DESIGN_RESOLUTION_640x960 1

#define TARGET_DESIGN_RESOLUTION_SIZE  DESIGN_RESOLUTION_640x960

typedef struct tagResources
{
	CCSize size;
	char directory[100];
}Resource;

static Resource smallResource = {CCSizeMake(320, 480), "small"};
static Resource highResource = {CCSizeMake(640, 960), "high"};

#if (TARGET_DESIGN_RESOLUTION_SIZE == DESIGN_RESOLUTION_320x480)
static CCSize designResolutionSize = CCSizeMake(320, 480);
static float catSize = 40;
#elif (TARGET_DESIGN_RESOLUTION_SIZE == DESIGN_RESOLUTION_640x960)
static CCSize designResolutionSize = CCSizeMake(640, 960);
static float catSize = 80;
#endif

#endif