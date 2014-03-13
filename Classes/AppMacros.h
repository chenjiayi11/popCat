#ifndef __APPMACROS_H__
#define __APPMACROS_H__

#include "cocos2d.h"
#include "MenuScene.h"

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
static int levelScore[] = {600,1800,3000,4200,6000,7800,9600,11400,13200,15000,17500,20000};
static bool isPause = false;
static int result_score;
static ccColor4F p_colors[5] = {ccc4f(236/255.f,105/255.f,136/255.f,1.0), ccc4f(68/255.f,201/255.f,220/255.f,1.0),
								ccc4f(55/255.f,209/255.f,101/255.f,1.0), ccc4f(157/255.f,108/255.f,209/255.f,1.0),
								ccc4f(206/255.f,205/255.f,77/255.f,1.0)};
static float explosion_interval = 0.1f;

#if (TARGET_DESIGN_RESOLUTION_SIZE == DESIGN_RESOLUTION_320x480)
static CCSize designResolutionSize = CCSizeMake(320, 480);
static float catSize = 40;
static int boxSize = 8;
#elif (TARGET_DESIGN_RESOLUTION_SIZE == DESIGN_RESOLUTION_640x960)
static CCSize designResolutionSize = CCSizeMake(640, 960);
static float catSize = 80;
static int boxSize = 8;
#endif

extern GameMenu* menuInstance;

#endif