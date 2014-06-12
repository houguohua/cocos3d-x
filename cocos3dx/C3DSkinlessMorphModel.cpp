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

#include "C3DSkinlessMorphModel.h"
#include "C3DMorphMesh.h"
#include "C3DMorph.h"

namespace cocos3d
{
C3DSkinlessMorphModel::C3DSkinlessMorphModel() :_morph(NULL)
{
}

C3DSkinlessMorphModel::~C3DSkinlessMorphModel()
{
	SAFE_DELETE(_morph);
}

C3DSkinlessMorphModel* C3DSkinlessMorphModel::create()
{
    return new C3DSkinlessMorphModel();
}

C3DMorph* C3DSkinlessMorphModel::getMorph()
{
	return _morph;
}

void C3DSkinlessMorphModel::setMorph(C3DMorph* morph)
{
	if(_morph != morph)
	{
		SAFE_DELETE(_morph);

		_morph = morph;
	}
}

void C3DSkinlessMorphModel::pushMorph(int morphTargetIndex,float weight)
{
	if(_mesh!=NULL && _morph!=NULL)
	{
		static_cast<C3DMorphMesh*>(_mesh)->pushMorph(_morph,morphTargetIndex,weight);
	}
}

void C3DSkinlessMorphModel::popMorph(int morphTargetIndex)
{
	if(_mesh!=NULL && _morph!=NULL)
	{
		static_cast<C3DMorphMesh*>(_mesh)->popMorph(_morph,morphTargetIndex);
	}
}

void C3DSkinlessMorphModel::changeMorph(int morphTargetIndex,float weight)
{
	if(_mesh!=NULL && _morph!=NULL)
	{
		static_cast<C3DMorphMesh*>(_mesh)->changeMorph(_morph,morphTargetIndex,weight);
	}
}

void C3DSkinlessMorphModel::copyFrom(const C3DSkinlessMorphModel* other)
{
    C3DSkinlessModel::copyFrom(other);

	setMorph(other->_morph);
}

C3DModel* C3DSkinlessMorphModel::clone(C3DNode::CloneContext& context) const
{
	C3DSkinlessMorphModel* other = new C3DSkinlessMorphModel();

	other->copyFrom(this);
	other->autorelease();
	return other;
}


}