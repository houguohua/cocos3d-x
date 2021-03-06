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

#include "Base.h"
#include "C3DAABB.h"
#include "C3DPlane.h"

namespace cocos3d
{
C3DAABB::C3DAABB()
{
}

C3DAABB::C3DAABB(const C3DVector3& min, const C3DVector3& max)
{
    set(min, max);
}

C3DAABB::C3DAABB(const C3DAABB& box)
{
	set(box._min,box._max);
}

C3DAABB::~C3DAABB()
{
}

C3DVector3 C3DAABB::getCenter()
{
    C3DVector3 center;
	center.x = 0.5f*(_min.x+_max.x);
	center.y = 0.5f*(_min.y+_max.y);
	center.z = 0.5f*(_min.z+_max.z);

    return center;
}

C3DVector3 C3DAABB::getSize()
{
	C3DVector3 size = _max - _min;

	return size;
}

float C3DAABB::getLength()
{
	C3DVector3 offset;

	offset.x = _max.x - _min.x;
	offset.y = _max.y - _min.y;
	offset.z = _max.z - _min.z;

	return offset.length();
}

void C3DAABB::getCorners(C3DVector3* dst) const
{
    assert(dst);

    // Near face, specified counter-clockwise looking towards the origin from the positive z-axis.
    // Left-top-front.
    dst[0].set(_min.x, _max.y, _max.z);
    // Left-bottom-front.
    dst[1].set(_min.x, _min.y, _max.z);
    // Right-bottom-front.
    dst[2].set(_max.x, _min.y, _max.z);
    // Right-top-front.
    dst[3].set(_max.x, _max.y, _max.z);

    // Far face, specified counter-clockwise looking towards the origin from the negative z-axis.
    // Right-top-back.
    dst[4].set(_max.x, _max.y, _min.z);
    // Right-bottom-back.
    dst[5].set(_max.x, _min.y, _min.z);
    // Left-bottom-back.
    dst[6].set(_min.x, _min.y, _min.z);
    // Left-top-back.
    dst[7].set(_min.x, _max.y, _min.z);
}

bool C3DAABB::intersects(const C3DAABB& box) const
{
    return ((_min.x >= box._min.x && _min.x <= box._max.x) || (box._min.x >= _min.x && box._min.x <= _max.x)) &&
           ((_min.y >= box._min.y && _min.y <= box._max.y) || (box._min.y >= _min.y && box._min.y <= _max.y)) &&
           ((_min.z >= box._min.z && _min.z <= box._max.z) || (box._min.z >= _min.z && box._min.z <= _max.z));
}

bool C3DAABB::containPoint( const C3DVector3& point) const
{
	if (point.x < _min.x) return false;
	if (point.y < _min.y) return false;
	if (point.z < _min.z) return false;
	if (point.x > _max.x) return false;
	if (point.y > _max.y) return false;
	if (point.z > _max.z) return false;
	return true;
}

bool C3DAABB::containSphere( const C3DVector3 &center,float radius ) const
{
	if (center.x+radius < _min.x) return false;
	if (center.y+radius < _min.y) return false;
	if (center.z+radius < _min.z) return false;
	if (center.x-radius > _max.x) return false;
	if (center.y-radius > _max.y) return false;
	if (center.z-radius > _max.z) return false;
	return true;
}

float C3DAABB::distance( const C3DVector3& vPoint)
{
	float fDist = 0;
	float m = 0;
	for(int i=0; i<3; ++i)
	{
		if(vPoint[i] < _min[i])
		{
			m = vPoint[i]-_min[i];
			fDist += m*m;
		}
		else if(vPoint[i] > _max[i])
		{
			m = _max[i]-vPoint[i];
			fDist += m*m;
		}
	}

	fDist = sqrtf(fDist);
	return fDist;
}

bool C3DAABB::intersects(const C3DPlane* plane,int type)
{
	C3DVector3 center = getCenter();
	float length = 0;

	float dis = fabsf(plane->dist2Plane(center));
	if(type == 0) //x
	{
		length = (_max.x - _min.x) * 0.5f;
	}
	else if(type == 1)//y
	{
		length = (_max.y - _min.y) * 0.5f;
	}
	else //z
	{
		length = (_max.z - _min.z) * 0.5f;
	}

	if(dis < length)
		return true;
	else
		return false;
}

void C3DAABB::merge(const C3DAABB& box)
{
    // Calculate the new minimum point.
    _min.x = std::min(_min.x, box._min.x);
    _min.y = std::min(_min.y, box._min.y);
    _min.z = std::min(_min.z, box._min.z);

    // Calculate the new maximum point.
    _max.x = std::max(_max.x, box._max.x);
    _max.y = std::max(_max.y, box._max.y);
    _max.z = std::max(_max.z, box._max.z);
}

void C3DAABB::set(const C3DVector3& min, const C3DVector3& max)
{
    this->_min = min;
    this->_max = max;
}

void C3DAABB::reset()
{
	_min.set(99999.0f, 99999.0f, 99999.0f);
	_max.set(-99999.0f, -99999.0f, -99999.0f);
}

void updateMinMax(C3DVector3* point, C3DVector3* min, C3DVector3* max)
{
    // Leftmost point.
    if (point->x < min->x)
    {
        min->x = point->x;
    }

    // Lowest point.
    if (point->y < min->y)
    {
        min->y = point->y;
    }

    // Farthest point.
    if (point->z < min->z)
    {
        min->z = point->z;
    }

    // Rightmost point.
    if (point->x > max->x)
    {
        max->x = point->x;
    }

    // Highest point.
    if (point->y > max->y)
    {
        max->y = point->y;
    }

    // Nearest point.
    if (point->z > max->z)
    {
        max->z = point->z;
    }
}

void C3DAABB::transform(const C3DMatrix& matrix)
{
    C3DVector3 corners[8];
	 // Near face, specified counter-clockwise
    // Left-top-front.
    corners[0].set(_min.x, _max.y, _max.z);
    // Left-bottom-front.
    corners[1].set(_min.x, _min.y, _max.z);
    // Right-bottom-front.
    corners[2].set(_max.x, _min.y, _max.z);
    // Right-top-front.
    corners[3].set(_max.x, _max.y, _max.z);

    // Far face, specified clockwise
    // Right-top-back.
    corners[4].set(_max.x, _max.y, _min.z);
    // Right-bottom-back.
    corners[5].set(_max.x, _min.y, _min.z);
    // Left-bottom-back.
    corners[6].set(_min.x, _min.y, _min.z);
    // Left-top-back.
    corners[7].set(_min.x, _max.y, _min.z);

    // C3DTransform the corners, recalculating the min and max points along the way.
    matrix.transformPoint(&corners[0]);
    C3DVector3 newMin = corners[0];
    C3DVector3 newMax = corners[0];
    for (int i = 1; i < 8; i++)
    {
        matrix.transformPoint(&corners[i]);
        updateMinMax(&corners[i], &newMin, &newMax);
    }
    _min = newMin;
    _max = newMax;
}
}