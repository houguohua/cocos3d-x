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

#include "C3DBone.h"

#include "Base.h"
#include "C3DAnimation.h"
#include "C3DAnimationChannel.h"
#include "C3DAnimationCurve.h"

#include "C3DAnimationClip.h"

namespace cocos3d
{
C3DBone::C3DBone(const std::string& id)
    : C3DNode(id), _jointMatrixDirty(true), _skinCount(0), _bSelect(false)/*, _animTarget(NULL)*/
{
	_animationChannels = NULL;
}

C3DBone::~C3DBone()
{
	//SAFE_RELEASE(_animTarget);

	if (_animationChannels)
    {
        std::vector<C3DAnimationChannel*>::iterator itr = _animationChannels->begin();
        while (itr != _animationChannels->end())
        {
            C3DAnimationChannel* channel = (*itr);
            channel->_animation->removeChannel(channel);
            SAFE_DELETE(channel);
            itr++;
        }
        _animationChannels->clear();
        SAFE_DELETE(_animationChannels);
    }
}

C3DBone* C3DBone::create(const std::string& id)
{
	C3DBone* bone = new C3DBone(id);
	bone->autorelease();
	return bone;
}

C3DNode::Type C3DBone::getType() const
{
	return C3DNode::NodeType_Bone;
}

void C3DBone::transformChanged()
{
    C3DNode::transformChanged();
    _jointMatrixDirty = true;
}

void C3DBone::updateJointMatrix(const C3DMatrix& bindShape, C3DVector4* matrixPalette)
{
    // Note: If more than one C3DMeshSkin influences this Joint, we need to skip
    // the _jointMatrixDirty optimization since updateJointMatrix() may be
    // called multiple times a frame with different bindShape matrices (and
    // different matrixPallete pointers).
    if (_skinCount > 1 || _jointMatrixDirty)
    {
        _jointMatrixDirty = false;

        static C3DMatrix t;
        C3DMatrix::multiply(C3DNode::getWorldMatrix(), getInverseBindPose(), &t);
        C3DMatrix::multiply(t, bindShape, &t);

        matrixPalette[0].set(t.m[0], t.m[4], t.m[8], t.m[12]);
        matrixPalette[1].set(t.m[1], t.m[5], t.m[9], t.m[13]);
        matrixPalette[2].set(t.m[2], t.m[6], t.m[10], t.m[14]);
    }
}

const C3DMatrix& C3DBone::getInverseBindPose()
{
    return _bindPose;
}

void C3DBone::setInverseBindPose(const C3DMatrix& m)
{
    _bindPose = m;
    _jointMatrixDirty = true;
}

C3DAnimation* C3DBone::getAnimation(const std::string& id)
{
   if (_animationChannels)
    {
        std::vector<C3DAnimationChannel*>::iterator itr = _animationChannels->begin();

        if (id.empty())
            return (*itr)->_animation;

        C3DAnimationChannel* channel = NULL;
        for (; itr != _animationChannels->end(); itr++)
        {
            channel = (C3DAnimationChannel*)(*itr);
            if (channel->_animation->_id.compare(id) == 0)
            {
                return channel->_animation;
            }
        }
    }

    return NULL;
}

void C3DBone::setAnimationValue( float* value, float blendWeight)
{
    assert(blendWeight >= 0.0f && blendWeight <= 1.0f);

	if(blendWeight == 0.0f)
		return;
	else if(blendWeight == 1.0f)
	{
		setScale(value[0], value[1], value[2]);
		applyAnimationValueRotation(value, 3, 1.0f);
		setPosition(value[7], value[8], value[9]);
	}
	else
	{
		setScale(C3DAnimationCurve::lerp(blendWeight, getScaleX(), value[0]), C3DAnimationCurve::lerp(blendWeight, getScaleY(), value[1]), C3DAnimationCurve::lerp(blendWeight, getScaleZ(), value[2]));
		applyAnimationValueRotation(value, 3, blendWeight);
		setPosition(C3DAnimationCurve::lerp(blendWeight, getPositionX(), value[7]), C3DAnimationCurve::lerp(blendWeight, getPositionY(), value[8]), C3DAnimationCurve::lerp(blendWeight, getPositionZ(), value[9]));
	}
}

void C3DBone::applyAnimationValueRotation(float* value, unsigned int index, float blendWeight)
{
    C3DQuaternion q(value[index], value[index + 1], value[index + 2], value[index + 3]);
    C3DQuaternion::slerp(_rotation, q, blendWeight, &q);
    setRotation(q);
}

void C3DBone::addChannel(C3DAnimationChannel* channel)
{
    if (_animationChannels == NULL)
        _animationChannels = new std::vector<C3DAnimationChannel*>;

    _animationChannels->push_back(channel);
}

void C3DBone::removeChannel(C3DAnimationChannel* channel)
{
    if (_animationChannels)
    {
        std::vector<C3DAnimationChannel*>::iterator itr = _animationChannels->begin();
        for ( ; itr != _animationChannels->end(); itr++)
        {
            C3DAnimationChannel* temp = *itr;
            if (channel == temp)
            {
                _animationChannels->erase(itr);

                if (_animationChannels->empty())
                    SAFE_DELETE(_animationChannels);

                return;
            }
        }
    }
}

C3DAnimationChannel* C3DBone::getChannel(const std::string& id) const
{
    if (_animationChannels)
    {
        std::vector<C3DAnimationChannel*>::iterator itr = _animationChannels->begin();

        if (id.empty())
            return (*itr);

        C3DAnimationChannel* channel = NULL;
        for (; itr != _animationChannels->end(); itr++)
        {
            channel = (C3DAnimationChannel*)(*itr);
            if (channel->_animation->_id.compare(id) == 0)
            {
                return channel;
            }
        }
    }

    return NULL;
}

void C3DBone::copyFrom(const C3DTransform* other, C3DNode::CloneContext& context)
{
    if (this == other)
        return;

    C3DNode::copyFrom(other, context);
    const C3DBone* otherNode = static_cast<const C3DBone*>(other);

	_id = otherNode->_id;;

    _bindPose = otherNode->_bindPose;
    _jointMatrixDirty = true;
    _skinCount = 0;
    if (otherNode->_animationChannels)
	{
		C3DAnimationChannel* channel = NULL;

		std::vector<C3DAnimationChannel*>::iterator itr = otherNode->_animationChannels->begin();
		for (; itr != otherNode->_animationChannels->end(); itr++)
		{
			channel = (C3DAnimationChannel*)(*itr);

			C3DAnimation* animation = NULL;
			std::map<const C3DAnimation*, C3DAnimation*>::iterator itor = context.clonedAnim.find(channel->_animation);
			if (itor == context.clonedAnim.end())
			{
				animation = channel->_animation->clone();
				context.clonedAnim[channel->_animation] = animation;
			}
			else
				animation = itor->second;
			C3DAnimationChannel* channelcopy = new C3DAnimationChannel(animation, this, channel->_curve, channel->_duration);
			animation->addChannel(channelcopy);
		}
	}
}

C3DNode* C3DBone::clone(C3DNode::CloneContext& context) const
{
	C3DBone* other = new C3DBone("");
	other->copyFrom(this, context);
	other->autorelease();
	return other;
}
}