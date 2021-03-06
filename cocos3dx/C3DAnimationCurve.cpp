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

#include "C3DAnimationCurve.h"
#include "C3DQuaternion.h"
#include "Base.h"

namespace cocos3d
{
C3DAnimationCurve* C3DAnimationCurve::create(unsigned int pointCount)
{
    return new C3DAnimationCurve(pointCount);
}

C3DAnimationCurve::C3DAnimationCurve(unsigned int pointCount)
    : _pointCount(pointCount), _componentSize(sizeof(float)*10),  _points(NULL), _dur(0)
{
    _points = new Point[_pointCount];
    for (unsigned int i = 0; i < _pointCount; i++)
    {
        _points[i].time = 0.0f;
        _points[i].value = new float[10];
    }
    _points[_pointCount - 1].time = 1.0f;
}

C3DAnimationCurve::~C3DAnimationCurve()
{
    for (unsigned int i = 0; i < _pointCount; i++) {
        SAFE_DELETE_ARRAY(_points[i].value);
    }
    SAFE_DELETE_ARRAY(_points);
}

C3DAnimationCurve::Point::Point()
    : time(0.0f), value(NULL)
{
}

C3DAnimationCurve::Point::~Point()
{
    SAFE_DELETE_ARRAY(value);
}

unsigned int C3DAnimationCurve::getPointCount() const
{
    return _pointCount;
}

float C3DAnimationCurve::getStartTime() const
{
    return _points[0].time;
}

float C3DAnimationCurve::getEndTime() const
{
    return _points[_pointCount-1].time;
}

void C3DAnimationCurve::setPoint(unsigned int index, float time, float* value)
{
    //assert(index < _pointCount && time >= 0.0f && time <= 1.0f && !(index == 0 && time != 0.0f) && !(_pointCount != 1 && index == _pointCount - 1 && time != 1.0f));

    _points[index].time = time;

    if (value)
        memcpy(_points[index].value, value, _componentSize);
}

void C3DAnimationCurve::evaluate(float time, float* dst, C3DAnimationCurve::InterpolationMode mode) const
{
    assert(dst && time >= 0 && time <= 1.0f);

    if (_pointCount == 1 || time <= _points[0].time)
    {
        memcpy(dst, _points[0].value, _componentSize);
        return;
    }
    else if (time >= _points[_pointCount - 1].time)
    {
        memcpy(dst, _points[_pointCount - 1].value, _componentSize);
        return;
    }

    unsigned int index = determineIndex(time);

    Point* from = _points + index;
    Point* to = _points + (index + 1);

    float scale = (to->time - from->time);
    float t = (time - from->time) / scale;

    if (mode == C3DAnimationCurve::Linear)
        interpolateLinear(t, from, to, dst);
    else if (mode == C3DAnimationCurve::Near)
    {
        if (t  < 0.5f)
            memcpy(dst, from->value, _componentSize);
        else
            memcpy(dst, to->value, _componentSize);
    }
}

float C3DAnimationCurve::lerp(float t, float from, float to)
{
    return from + (to-from) * t;
}

void C3DAnimationCurve::interpolateLinear(float t, Point* from, Point* to, float* dst) const
{
    float* fromValue = from->value;
    float* toValue = to->value;

    unsigned int i = 0;
    for (i = 0; i < 3; i++)
    {
        if (fromValue[i] == toValue[i])
            dst[i] = fromValue[i];
        else
            dst[i] = fromValue[i] + (toValue[i] - fromValue[i]) * t;
    }

    interpolateQuaternion(t, (fromValue + i), (toValue + i), (dst + i));

    for (i += 4; i < 10; i++)
    {
        if (fromValue[i] == toValue[i])
            dst[i] = fromValue[i];
        else
            dst[i] = fromValue[i] + (toValue[i] - fromValue[i]) * t;
    }
}

void C3DAnimationCurve::interpolateQuaternion(float t, float* from, float* to, float* dst) const
{
    // Evaluate.
    if (t >= 0)
    {
        C3DQuaternion::slerp(from[0], from[1], from[2], from[3], to[0], to[1], to[2], to[3], t, dst, dst + 1, dst + 2, dst + 3);
    }
    else
        C3DQuaternion::slerp(to[0], to[1], to[2], to[3], from[0], from[1], from[2], from[3], t, dst, dst + 1, dst + 2, dst + 3);

    //((C3DQuaternion*) dst)->normalize();
}

int C3DAnimationCurve::determineIndex(float time) const
{
    unsigned int min = 0;
    unsigned int max = _pointCount - 1;
    unsigned int mid = 0;

    do
    {
        mid = (min + max) >> 1;

        if (time >= _points[mid].time && time <= _points[mid + 1].time)
            return mid;
        else if (time < _points[mid].time)
            max = mid - 1;
        else
            min = mid + 1;
    } while (min <= max);

    // We should never hit this!
    return -1;
}

    ///////////////////implementation of animation manager////////////

    C3DAnimationCurveMgr* C3DAnimationCurveMgr::_instance = NULL;
    C3DAnimationCurveMgr::C3DAnimationCurveMgr()
    {
    }
    C3DAnimationCurveMgr::~C3DAnimationCurveMgr()
    {
        for (CurvesMap::iterator it = _curves.begin(); it != _curves.end(); it++)
		{
           // removeAnimationCurves(it->first.c_str() );
            CurveMap* curvemap = it->second;
            for (CurveMap::iterator itor = curvemap->begin(); itor != curvemap->end(); itor++) {
                itor->second->release();
            }

			curvemap->clear();
            delete curvemap;
        }
        _curves.clear();
    }

    C3DAnimationCurveMgr* C3DAnimationCurveMgr::sharedAnimationCurveMgr()
    {
        if (_instance == NULL)
        {
            static C3DAnimationCurveMgr mgr;
            _instance = &mgr;
        }

        return _instance;
    }

    const C3DAnimationCurve* C3DAnimationCurveMgr::getAnimationCurve(const std::string& path, const std::string& boneId)
    {
        CurvesMap::iterator it = _curves.find(path);
        if (it != _curves.end())
        {
            CurveMap* curvemap = it->second;
            CurveMap::iterator itor = curvemap->find(boneId);
            if (itor != curvemap->end())
                return itor->second;
        }

        return NULL;
    }
    C3DAnimationCurveMgr::CurveMap* C3DAnimationCurveMgr::getAnimationCurves(const std::string& path)
    {
        CurvesMap::iterator it = _curves.find(path);
        if (it != _curves.end())
        {
            return it->second;
        }
        return NULL;
    }
    void C3DAnimationCurveMgr::addAnimationCurve(const std::string& path, const std::string& boneId, C3DAnimationCurve* curve)
    {
        CurveMap* &curvemap = _curves[path];
        if (curvemap == NULL)
            curvemap = new CurveMap();

        curve->retain();
        curvemap->insert(std::pair<std::string, C3DAnimationCurve*>(boneId, curve) );
    }
    void C3DAnimationCurveMgr::addAnimationCurves(const std::string& path, CurveMap* curvemap)
    {
        removeAnimationCurves(path);
        _curves.insert(std::pair<std::string, CurveMap*>(path, curvemap) );
        for (CurveMap::iterator itor = curvemap->begin(); itor != curvemap->end(); itor++) {
            itor->second->retain();
        }
    }
    C3DAnimationCurveMgr::CurveMap* C3DAnimationCurveMgr::createAnimationCurves(const std::string& path)
    {
        removeAnimationCurves(path);
        CurveMap* curvemap = new CurveMap();
        _curves.insert(std::pair<std::string, CurveMap*>(path, curvemap) );
        return curvemap;
    }
    void C3DAnimationCurveMgr::removeAnimationCurve(const std::string& path, const std::string& boneId)
    {
        CurvesMap::iterator it = _curves.find(path);
        if (it != _curves.end())
        {
            CurveMap* curvemap = it->second;
            CurveMap::iterator itor = curvemap->find(boneId);
            if (itor != curvemap->end())
            {
                itor->second->release();
                curvemap->erase(itor);
            }
        }
    }
    void C3DAnimationCurveMgr::removeAnimationCurves(const std::string& path)
    {
        CurvesMap::iterator it1 = _curves.find(path);
        if (it1 != _curves.end())
        {
            CurveMap* curvemap = it1->second;
            for (CurveMap::iterator itor = curvemap->begin(); itor != curvemap->end(); itor++) {
                itor->second->release();
            }
            _curves.erase(it1);
        }
    }

    C3DAnimationCurve* C3DAnimationCurveMgr::createAniamationCurve(unsigned int keyCount, unsigned long* keyTimes, float* keyValues)
    {
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

        return curve;
    }
}