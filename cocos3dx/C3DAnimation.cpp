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

#include "C3DAnimation.h"
#include "Base.h"
#include "C3DAnimationClip.h"

#include "C3DAnimationChannel.h"
#include "C3DAnimationCurve.h"
#include "C3DRenderSystem.h"
#include "C3DProfile.h"

namespace cocos3d
{
C3DAnimation::C3DAnimation(const std::string& id)
    : _id(id), _duration(0),
	 _defaultClip(NULL), _clips(NULL),_currentClip(NULL), _state(STOPPED), _quality(C3DAnimation::High)
{
}

C3DAnimation::~C3DAnimation()
{
    _channels.clear();

    if (_defaultClip)
    {
        SAFE_RELEASE(_defaultClip);
    }

	if(_runningClips.size() > 0)
	{
		std::list<C3DAnimationClip*>::iterator clipIter = _runningClips.begin();

        while (clipIter != _runningClips.end())
        {
            C3DAnimationClip* clip = *clipIter;
            SAFE_RELEASE(clip);
            clipIter++;
        }
        _runningClips.clear();
    }

    if (_clips)
    {
        std::vector<C3DAnimationClip*>::iterator clipIter = _clips->begin();

        while (clipIter != _clips->end())
        {
            C3DAnimationClip* clip = *clipIter;
            SAFE_RELEASE(clip);
            clipIter++;
        }
        _clips->clear();
    }
    SAFE_DELETE(_clips);
}

C3DAnimation* C3DAnimation::create(const std::string& id)
{
	C3DAnimation* animation = new C3DAnimation(id);
	animation->autorelease();
	return animation;
}

const std::string& C3DAnimation::getId() const
{
    return _id;
}

unsigned long C3DAnimation::getDuration() const
{
    return _duration;
}

C3DAnimationClip* C3DAnimation::createClip(const std::string& id, unsigned long start, unsigned long end)
{
	C3DAnimationClip* clip = new C3DAnimationClip(id, this, start, end);

	clip->autorelease();

    return clip;
}

C3DAnimationClip* C3DAnimation::getClip(const std::string& id)
{
    // If id is NULL return the default clip.
    if (id.empty())
    {
        if (_defaultClip == NULL)
            createDefaultClip();

        return _defaultClip;
    }
    else
    {
        return findClip(id);
    }
}

C3DAnimationClip* C3DAnimation::getClip(unsigned int index) const
{
    if (_clips)
        return _clips->at(index);

    return NULL;
}

unsigned int C3DAnimation::getClipCount() const
{
    return _clips ? _clips->size() : 0;
}

void C3DAnimation::setFrameCount(unsigned int frameCount)
{
	_frameCount = frameCount;
}

unsigned int C3DAnimation::getFrameCount()
{
	return _frameCount;
}

void C3DAnimation::play(C3DAnimationClip* clip)
{
    if (clip == _currentClip && clip->isPlaying())
        return;

	if(clip == _currentClip && clip->isPaused())
		return;

	if(clip == _currentClip && clip->isResumed())
		return;

	if(_currentClip != NULL && clip != _currentClip && _currentClip->isPaused())
	{
		_currentClip->resetState(C3DAnimationClip::CLIP_IS_PAUSED);
		removeRunClip(_currentClip);
		clip->play();
	}
	else if(_currentClip != NULL && _currentClip->isPlaying())
	{
		_currentClip->crossFade(clip, 100);
	}
    else
    {
        clip->play();
    }

    _currentClip = clip;
}

void C3DAnimation::play(const std::string& clipId)
{
    // If id is NULL, play the default clip.
    if (clipId.empty())
    {
        if (_defaultClip == NULL)
            createDefaultClip();

		play(_defaultClip);
    }
    else
    {
        // Find animation clip.. and play.
        C3DAnimationClip* clip = findClip(clipId);
        if(clip != NULL)
			play(clip);
    }
}

void C3DAnimation::stop(const std::string& clipId)
{
    // If id is NULL, play the default clip.
    if (clipId.empty())
    {
        if (_defaultClip)
            _defaultClip->stop();
    }
    else
    {
        // Find animation clip.. and play.
        C3DAnimationClip* clip = findClip(clipId);
        if (clip != NULL)
            clip->stop();
    }
}

void C3DAnimation::pause(const std::string& clipId)
{
    if (clipId.empty())
    {
        if (_defaultClip)
            _defaultClip->pause();
    }
    else
    {
        C3DAnimationClip* clip = findClip(clipId);
        if (clip != NULL)
            clip->pause();
    }
}

void C3DAnimation::resume(const std::string& clipId)
{
    if (clipId.empty())
    {
        if (_defaultClip)
            _defaultClip->pause();
    }
    else
    {
        C3DAnimationClip* clip = findClip(clipId);
        if (clip != NULL)
            clip->resume();
    }
}

void C3DAnimation::createDefaultClip()
{
    _defaultClip = new C3DAnimationClip("default_clip", this, 0.0f, _duration);
}

C3DAnimationClip* C3DAnimation::addClip(const std::string& name, unsigned int startFrame, unsigned int endFrame, float repeatCount, float speed)
{
	if (name.empty())
		return NULL;

	C3DAnimationClip* clip = findClip(name);
	if (clip)return NULL;

	clip = createClip(name, ((float) startFrame / _frameCount) * _duration, ((float) endFrame / _frameCount) * _duration);

    clip->setRepeatCount(repeatCount);
    clip->setSpeed(speed);

	addClip(clip);

	return clip;
}

bool C3DAnimation::replaceClip(const std::string& name, unsigned int startFrame, unsigned int endFrame, float repeatCount, float speed)
{
	removeClip(name);
	C3DAnimationClip* clip = addClip(name, startFrame, endFrame, repeatCount, speed);
	if (clip)
		return true;
	else
		return false;
}

bool C3DAnimation::removeClip(const std::string& name)
{
	std::vector<C3DAnimationClip*>::iterator clipIter = _clips->begin();
	while (clipIter != _clips->end())
	{
		C3DAnimationClip* clip = *clipIter;
		if (clip->getID()==name)
		{
			_clips->erase(clipIter);
			SAFE_DELETE(clip);
			return true;
		}
		clipIter++;
	}
	return false;
}

void C3DAnimation::addClip(C3DAnimationClip* clip)
{
    if (_clips == NULL)
        _clips = new std::vector<C3DAnimationClip*>;

	clip->retain();

    _clips->push_back(clip);
}

C3DAnimationClip* C3DAnimation::findClip(const std::string& id) const
{
    if (_clips)
    {
        C3DAnimationClip* clip = NULL;
        unsigned int clipCount = _clips->size();
        for (unsigned int i = 0; i < clipCount; i++)
        {
            clip = _clips->at(i);
            if (clip->_id.compare(id) == 0)
            {
                return _clips->at(i);
            }
        }
    }
    return NULL;
}

C3DAnimationChannel* C3DAnimation::createChannel(C3DBone* bone, unsigned int keyCount, unsigned long* keyTimes, float* keyValues)
{
	setFrameCount(keyCount);

    C3DAnimationCurve* curve = C3DAnimationCurve::create(keyCount);

    unsigned long lowest = keyTimes[0];
    unsigned long duration = keyTimes[keyCount-1] - lowest;

    float keytime;

    keytime = 0.0f;
    curve->setPoint(0, keytime, keyValues);

    unsigned int pointOffset = 10;
    unsigned int i = 1;
    for (; i < keyCount - 1; i++)
    {
        keytime = (float) (keyTimes[i] - lowest) / (float) duration;
        curve->setPoint(i, keytime, (keyValues + pointOffset));
        pointOffset += 10;
    }
    i = keyCount - 1;
    keytime = 1.0f;
    curve->setPoint(i, keytime, keyValues + pointOffset);

    curve->_dur = duration;

    C3DAnimationChannel* channel = new C3DAnimationChannel(this, bone,curve, duration);
    addChannel(channel);
    return channel;
}

C3DAnimationChannel* C3DAnimation::createChannel(C3DBone* bone, C3DAnimationCurve* curve)
{
    //setFrameCount(curve->getPointCount());

    C3DAnimationChannel* channel = new C3DAnimationChannel(this, bone,curve, curve->getDruationTime());
    addChannel(channel);
    return channel;
}

void C3DAnimation::addChannel(C3DAnimationChannel* channel)
{
    _channels.push_back(channel);

    if (channel->_duration > _duration)
        _duration = channel->_duration;
}

void C3DAnimation::removeChannel(C3DAnimationChannel* channel)
{
    std::vector<C3DAnimationChannel*>::iterator itr = _channels.begin();
    while (itr != _channels.end())
    {
        C3DAnimationChannel* chan = *itr;
        if (channel == chan)
        {
            _channels.erase(itr);
            return;
        }
        else
        {
            itr++;
        }
    }
}

C3DAnimation* C3DAnimation::clone()
{
    C3DAnimation* animation = new C3DAnimation(getId());

	animation->_duration = _duration;
	animation->_frameCount = _frameCount;
    animation->_quality = _quality;

	for( unsigned int k = 0; k < getClipCount(); k++)
	{
		C3DAnimationClip* clipSource = getClip(k);
		C3DAnimationClip* clip = clipSource->clone(animation);
		animation->addClip(clip);
	}

	std::list<C3DAnimationClip*>::iterator clipIter = _runningClips.begin();
    while (clipIter != _runningClips.end())
    {
        C3DAnimationClip* srcClip = *clipIter;
		C3DAnimationClip* dstClip = animation->findClip(srcClip->getID());
		if(dstClip != NULL)
		{
			animation->addRunClip(dstClip);
		}
        clipIter++;
    }

	animation->autorelease();

    return animation;
}

void C3DAnimation::stopAll()
{
    std::list<C3DAnimationClip*>::iterator clipIter = _runningClips.begin();
    while (clipIter != _runningClips.end())
    {
        C3DAnimationClip* clip = *clipIter;
        clip->stop();
        clipIter++;
    }
}

C3DAnimation::State C3DAnimation::getState() const
{
    return _state;
}

void C3DAnimation::resumeAll()
{
    if (_runningClips.empty())
        _state = IDLE;
    else
        _state = RUNNING;
}

void C3DAnimation::pauseAll()
{
    _state = PAUSED;
}

void C3DAnimation::addRunClip(C3DAnimationClip* clip)
{
    if (!clip) return;

    // if the animation-clip has in the _runingClips, return.
    std::list<C3DAnimationClip*>::iterator clipItr = _runningClips.begin();
    while (clipItr != _runningClips.end())
    {
        if (clip->getID() == (*clipItr)->getID())
            return;
        else
            ++clipItr;
    }

    if (_runningClips.empty())
    {
        _state = RUNNING;
    }

    clip->retain();
    _runningClips.push_back(clip);
}

void C3DAnimation::removeRunClip(C3DAnimationClip* clip)
{
    std::list<C3DAnimationClip*>::iterator clipItr = _runningClips.begin();
    while (clipItr != _runningClips.end())
    {
        C3DAnimationClip* rClip = (*clipItr);
        if (rClip == clip)
        {
            _runningClips.erase(clipItr);
            SAFE_RELEASE(clip);
            break;
        }
        clipItr++;
    }

    if (_runningClips.empty())
        _state = IDLE;
}

void C3DAnimation::update(long elapsedTime, bool updatePose)
{
    BEGIN_PROFILE("3danimation");
    if (_state != RUNNING)
        return;

    std::list<C3DAnimationClip*>::iterator clipIter = _runningClips.begin();
    while (clipIter != _runningClips.end())
    {
        C3DAnimationClip* clip = (*clipIter);

		unsigned short res = clip->update(elapsedTime, updatePose);

		if(res == C3DAnimationClip::CLIP_IS_MARKED_FOR_REMOVAL)
		{
			SAFE_RELEASE(clip);
            clipIter = _runningClips.erase(clipIter);
		}
		else if(res == C3DAnimationClip::CLIP_IS_PAUSED)
		{
			clipIter++;
		}
		else if(res == C3DAnimationClip::CLIP_IS_RESUME)
		{
		}
		else if(res == C3DAnimationClip::CLIP_IS_RESTARTED)
		{
			clipIter++;
		}
        else
        {
            clipIter++;
        }
    }

    if (_runningClips.empty())
        _state = IDLE;

    END_PROFILE("3danimation");
}
}