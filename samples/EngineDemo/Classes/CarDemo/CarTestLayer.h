/****************************************************************************
Copyright (c) Chukong Technologies Inc.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/

#ifndef _CARTESTLAYER_H_ 
#define _CARTESTLAYER_H_

#include "CCNode.h"
//#include "touch_dispatcher/CCTouchDelegateProtocol.h"
#include "cocos2d.h"
#include "cocos3d.h"
#include "TestLayer.h"

USING_NS_CC;

class CarTestLayer : public TestLayer
{
public:
	CarTestLayer();
	virtual ~CarTestLayer();

	virtual bool init();   

	virtual void update(float dt);

	virtual void draw3D(void);//render the 3d contents

	CREATE_FUNC(CarTestLayer);


	void setUpScene();

	void setUpCar();

	// setup initialize camera
	void setUpCamera();
	//setup initialize light
	void setUpLight();

	virtual void touchEvent(cocos3d::TouchEvent evt, float x, float y, unsigned int contactIndex);


	void menuCallback(CCObject * pSender);

	CCLayer* createUILayer();

protected:
	bool _touched;
	int _touchX;
	int _touchY;

	cocos3d::C3DStaticObj* _sm;
	cocos3d::C3DSprite* _obj;

};

#endif