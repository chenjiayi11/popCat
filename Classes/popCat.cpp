/*
 * Copyright (C) 2009 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */
#include <string.h>
#include <jni.h>
#include "cocos2d.h"
#include  "AppMacros.h"


#ifdef __cplusplus
extern "C" {
#endif
void
Java_com_taobao_popcat_popCat_ShowScoreList( JNIEnv* env,
                                                  jobject thiz, jobjectArray array)
{
	CCLOG("Java_com_taobao_popcat_popCat_ShowScoreList called");
	if(menuInstance)
	{
		menuInstance->showList(env, array);
	}
}

#ifdef __cplusplus
}
#endif
