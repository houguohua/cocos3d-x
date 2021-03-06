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

#ifndef __MainPlayer_H__
#define __MainPlayer_H__

#include "C3DActor.h"
#include "C3DVector2.h"
#include <list>

namespace cocos3d
{
	class C3DSprite;
	class C3DVector2;
	class C3DCamera;
	class C3DStaticObj;
}

enum EAttackType
{
	AT_MeleeMask = 0x01,
	AT_RangeMask = 0x10,
	AT_Shift = 2,

	AT_Melee	= (1<<AT_Shift) | AT_MeleeMask,	// 近战
	AT_Range	= (2<<AT_Shift) | AT_RangeMask,	// 远程
	AT_Lighting	= (3<<AT_Shift) | AT_RangeMask,	// 闪电
};

namespace cocos2d
{
class BaseBullet;
class MainPlayer : public C3DActor
{
public:
	MainPlayer(std::string& name,cocos3d::C3DNode* node,cocos3d::C3DLayer* layer);
	virtual ~MainPlayer();

	class AnimListenerObject : public cocos3d::ListenerObject
	{
	public:
		AnimListenerObject(MainPlayer* mainPlayer);
		void onMeleeAttackEnd();
		void onRemoteAttackEnd();

	protected:
		MainPlayer* _mainPlayer;
	};

	enum State
	{
		State_None = 0,
		State_Idle = 0x01,
		State_Move = 0x02,
		State_Rotate = 0x04,
		State_Speak = 0x08,
		State_MeleeAttack = 0x10,
		State_RemoteAttack = 0x20,

		State_Attack = 0x40,
	};

	virtual C3DActor::Type getType();

	virtual void init();

	virtual void moveTo(cocos3d::C3DVector3& target);

	virtual void update(long elapsedTime);

	void speak(C3DActor* targetActor);
	void meleeAttack(C3DActor* targetActor);
	void meleeAttack();
	void remoteAttack(C3DActor* targetActor);

	bool isState(unsigned int state,unsigned int bit) const
	{
		return (state & bit) == bit;
	}

	C3DActor* getTargetActor()
	{
		return _targetActor;
	}

	void setAttackType( EAttackType attackType )
	{
		_attackType = attackType;
	}

	EAttackType getAttackType(void)
	{
		return _attackType;
	}

private:
	void updateState(long elapsedTime);
	void move2D(long elapsedTime);
	void move3D(long elapsedTime);
	void rotate(long elapsedTime);

private:
	C3DActor* _targetActor;
	cocos3d::C3DVector2 _velocity;
	unsigned int _lastState;
	unsigned int _curState;
	MainPlayer::AnimListenerObject* _animListener;

	cocos3d::C3DParticleSystem* _particle;
	//Bullet* _bullet;
	std::list<BaseBullet*> _bullets;

	EAttackType _attackType;

	cocos3d::C3DCamera* _camera;
	cocos3d::C3DVector3 _offset;

	cocos3d::C3DStaticObj* _terrainFlag;
};

}
#endif
