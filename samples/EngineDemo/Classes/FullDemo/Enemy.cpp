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

#include "Enemy.h"

#include "C3DSprite.h"
#include "C3DMaterial.h"
#include "MaterialParameter.h"
#include "C3DVector3.h"
#include "C3DTechnique.h"
#include "C3DPass.h"

using namespace cocos3d;

namespace cocos2d
{
void Enemy::AnimListenerObject::onHurtEnd()
{
	if (_player)
	{
		_player->_lastState = Enemy::State_Idle;
	    _player->_curState = Enemy::State_Idle;
	}
}

void Enemy::AnimListenerObject::onInjureEnd()
{
	if (_player)
	{
	    _player->_curState = Enemy::State_Idle;
	}
}

void Enemy::AnimListenerObject::onStandupEnd()
{
	if (_player)
	{
		_player->_life = 30;

	    _player->_curState = Enemy::State_Idle;
	}
}

Enemy::Enemy(std::string& name,cocos3d::C3DNode* node,cocos3d::C3DLayer* layer):C3DActor(name,node,layer), _hurtTime(0L),_hurtRandom(0.f)
{
	_animListener = new AnimListenerObject(this);

	C3DAnimationClip* clip = (static_cast<cocos3d::C3DSprite*>(_node))->getAnimationClip("injure");
	if(clip != NULL)
	{
		cocos3d::C3DActionListener* endAction = cocos3d::C3DActionListener::create(_animListener,((cocos3d::ListenerFunction)(&cocos2d::Enemy::AnimListenerObject::onInjureEnd)));
		clip->addActionEvent(endAction,clip->getDuration());
	}

	clip = (static_cast<cocos3d::C3DSprite*>(_node))->getAnimationClip("vertigo");
	if(clip != NULL)
	{
		cocos3d::C3DActionListener* endAction = cocos3d::C3DActionListener::create(_animListener,((cocos3d::ListenerFunction)(&cocos2d::Enemy::AnimListenerObject::onInjureEnd)));
		clip->addActionEvent(endAction,clip->getDuration());
	}

	clip = (static_cast<cocos3d::C3DSprite*>(_node))->getAnimationClip("standup");
	if(clip != NULL)
	{
		cocos3d::C3DActionListener* endAction = cocos3d::C3DActionListener::create(_animListener,((cocos3d::ListenerFunction)(&cocos2d::Enemy::AnimListenerObject::onStandupEnd)));
		clip->addActionEvent(endAction,clip->getDuration());
	}

	_lastState = Enemy::State_Idle;
	_curState = Enemy::State_Idle;
}

Enemy::~Enemy()
{
	delete (_animListener);
}

void Enemy::init()
{
}

C3DActor::Type Enemy::getType()
{
	return C3DActor::ActorType_Enemy;
}

static float dieTime = 0;
void Enemy::updateState(long elapsedTime)
{
	if(_lastState == _curState && isState(_curState,Enemy::State_Die))
	{
		dieTime += elapsedTime;
		if(dieTime > 20000.0f)
		{
			dieTime = 0;
			_curState = Enemy::State_Standup;
		}
	}
	else if(isState(_lastState,Enemy::State_Injure))
	{
		if(isState(_curState,Enemy::State_Idle))
		{
		//	_lastState = Enemy::State_Idle;
		}
		else
		{
			if(_life <= 0)
				_curState = Enemy::State_Die;
			else
				_curState = Enemy::State_Injure;
		}
	}
}

void Enemy::update(long elapsedTime)
{
	updateState(elapsedTime);

	/*if(isState(_lastState,Enemy::State_Idle) && isState(_curState,Enemy::State_Idle))
		return;*/

	/*if(_lastState == _curState && isState(_curState,Enemy::State_Die))
		return;*/

	if(isState(_lastState,Enemy::State_Injure) )
	{
		if(isState(_curState,Enemy::State_Injure))
		{
			C3DSprite* sprite = ((C3DSprite*)this->_node);

			if(_hurtRandom > 0.5f)
				(static_cast<cocos3d::C3DSprite*>(_node))->playAnimationClip("injure");
			else
				(static_cast<cocos3d::C3DSprite*>(_node))->playAnimationClip("vertigo");
			_lastState = Enemy::State_Injure;

			MaterialParameter* mp = sprite->getMaterial("body")->getTechnique(0u)->getPass(0u)->getParameter("u_diffuseColor");
			mp->setValue(C3DVector4(0.8f, 0.3f, 0.3f, 1.0f));
		}
		else if(isState(_curState,Enemy::State_Die))
		{
			C3DSprite* sprite = ((C3DSprite*)this->_node);
			(static_cast<cocos3d::C3DSprite*>(_node))->playAnimationClip("die");
			_lastState = Enemy::State_Die;

			MaterialParameter* mp = sprite->getMaterial("body")->getTechnique(0u)->getPass(0u)->getParameter("u_diffuseColor");
			mp->setValue(C3DVector4(0.8f, 0.3f, 0.3f, 1.0f));
		}
		else if(isState(_curState,Enemy::State_Idle))
		{
			C3DSprite* sprite = ((C3DSprite*)this->_node);
			MaterialParameter* mp = sprite->getMaterial("body")->getTechnique(0u)->getPass(0u)->getParameter("u_diffuseColor");
			mp->setValue(C3DVector4(1.0f, 1.0f, 1.0f, 1.0f));
			sprite->playAnimationClip("idle");
			_lastState = Enemy::State_Idle;
		}
	}
	else if(isState(_lastState,Enemy::State_Die))
	{
		if(isState(_curState,Enemy::State_Standup))
		{
			C3DSprite* sprite = ((C3DSprite*)this->_node);
			(static_cast<cocos3d::C3DSprite*>(_node))->playAnimationClip("standup");
			_lastState = Enemy::State_Standup;

			MaterialParameter* mp = sprite->getMaterial("body")->getTechnique(0u)->getPass(0u)->getParameter("u_diffuseColor");
			mp->setValue(C3DVector4(1.0f, 1.0f, 1.0f, 1.0f));
		}
	}
    else if (isState(_lastState,Enemy::State_Standup))
    {
        if(isState(_curState,Enemy::State_Idle))
        {
            C3DSprite* sprite = ((C3DSprite*)this->_node);
            MaterialParameter* mp = sprite->getMaterial("body")->getTechnique(0u)->getPass(0u)->getParameter("u_diffuseColor");
            mp->setValue(C3DVector4(1.0f, 1.0f, 1.0f, 1.0f));
            sprite->playAnimationClip("idle");
            _lastState = Enemy::State_Idle;
        }
    }
}

void Enemy::hurt(float damage, long hurtTime)
{
	_lastState = Enemy::State_Injure;
	_curState = Enemy::State_Injure;
	_hurtTime = hurtTime;
	_life -= damage;
	_hurtRandom = CCRANDOM_0_1();
}
}