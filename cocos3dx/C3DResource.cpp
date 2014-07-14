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

#include "C3DResource.h"
#include "C3DElementNode.h"
#include "C3DResourcePool.h"
#include "StringTool.h"
namespace cocos3d
{
C3DResource::C3DResource(): _id(""),_resourceName(""),_size(0)
{
	_checkWaitTime = 0;
	_waitTime = 100;
	_state = State_Init;
	_cloneNum = 0;
}

C3DResource::C3DResource(const std::string& id): _id(id),_resourceName(id),_size(0)
{
	_checkWaitTime = 0;
	_waitTime = 100;
	_state = State_Init;
	_cloneNum = 0;
}

C3DResource::~C3DResource()
{
}

size_t C3DResource::getSize(void) const
{
    return _size;
}

const std::string& C3DResource::getID(void) const
{
    return _id;
}

void C3DResource::setID(std::string& id)
{
	_id = id;
}

const C3DResource::State C3DResource::getState(void) const
{
	return _state;
}

const std::string& C3DResource::getResourceName(void) const
{
	return _resourceName;
}

void C3DResource::setState(C3DResource::State state)
{
	_state = state;
}

bool C3DResource::load(C3DElementNode* node)
{
	return false;
}

bool C3DResource::save(C3DElementNode* node)
{
	return false;
}

void C3DResource::update(long elapsedTime)
{
	switch(_state)
	{
	case C3DResource::State_Init:
		{
			if(this->getReferenceCount()>1)
			{
				_state = State_Used;
			}
		}
		break;
    default:
        break;
	}
}

void C3DResource::copyFrom(const C3DResource* other)
{
	const_cast<C3DResource*>(other)->_cloneNum += 1;
	_cloneNum = 0;

	_resourceName = other->_resourceName;
	_id = other->_id + "_" + StringTool::toString(other->_cloneNum);
	_size = other->_size;
	_state = C3DResource::State_Init;
	_waitTime = other->_waitTime;
}

C3DResource* C3DResource::clone() const
{
	C3DResource* other = new C3DResource("");

	other->_resourceName = _resourceName;
	other->_id = _id;
	other->_size = _size;
	other->_state = C3DResource::State_Init;
	other->_waitTime = _waitTime;

	other->autorelease();
	return other;
}

int C3DResource::getCloneNum()
{
	return _cloneNum;
}
}