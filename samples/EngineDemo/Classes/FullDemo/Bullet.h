﻿/****************************************************************************
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

#ifndef __Bullet_H__
#define __Bullet_H__

#include "BaseBullet.h"

namespace cocos3d
{
	class C3DSprite;
	class C3DVector2;
}

namespace cocos2d
{
class Bullet : public BaseBullet
{
public:
	Bullet(std::string& name,cocos3d::C3DNode* node,cocos3d::C3DLayer* layer);
	virtual ~Bullet();

	//virtual void init();

	//virtual void moveTo(cocos3d::C3DVector3& target);

	//virtual void update(long elapsedTime);

	//virtual void spell(C3DActor* caster, C3DActor* target);

protected:
	//void updateState(long elapsedTime);
	//void move3D(long elapsedTime);

	virtual void onSpell(void);
	virtual void onFly(void);
	virtual void onHit(void);
	virtual void onFinish(void);

	virtual void stateSpell(long elapsedTime);
	virtual void stateFly(long elapsedTime);
	virtual void stateHit(long elapsedTime);
	virtual void stateFinish(long elapsedTime);

private:

	cocos3d::C3DParticleSystem* _moveParticle;
	cocos3d::C3DParticleSystem* _explodeParticle;

	float _hitTime;
};

}
#endif
