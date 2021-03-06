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

#include "C3DAnimatedLight.h"
#include "C3DTexture.h"
#include "C3DSampler.h"

namespace cocos3d
{
C3DAnimatedLight::C3DAnimatedLight() : C3DLightComponent(C3DLight::ANIMATE), _sampler(NULL), _scaleUV(1, 1, 1, 1)
{
	updateRotScale();
}

C3DAnimatedLight::~C3DAnimatedLight()
{
	SAFE_RELEASE(_sampler);
}

void C3DAnimatedLight::setSpeedUV(float speedU1, float speedV1, float speedU2, float speedV2)
{
	_speedUV.set(speedU1, speedV1, speedU2, speedV2);
}

void C3DAnimatedLight::setScaleUV(float scaleU1, float scaleV1, float scaleU2, float scaleV2)
{
	_scaleUV.set(scaleU1, scaleV1, scaleU2, scaleV2);
	updateRotScale();
}

void C3DAnimatedLight::setRotateUV(float rotate1, float rotate2)
{
	_rotateUV.set(rotate1, rotate2);
	updateRotScale();
}

void C3DAnimatedLight::update(unsigned int deltatime)
{
	float dt = deltatime * 0.001f;

	_offsetUV += _speedUV * dt;

	if (!_speedUV.isZero())
	{
		_rotateUV += _rotateSpeedUV * dt;
		updateRotScale();
	}
}

void C3DAnimatedLight::setTexture(C3DTexture* texture)
{
	SAFE_RELEASE(_sampler);

	_sampler = C3DSampler::create(texture);
	_sampler->setWrapMode(Texture_Wrap_REPEAT, Texture_Wrap_REPEAT);
}

void C3DAnimatedLight::updateRotScale()
{
	float cosRot = cos(_rotateUV.x);
	float sinRot = sin(_rotateUV.x);

	_uv1RotScale.set(cosRot * _scaleUV.x, -sinRot * _scaleUV.y, sinRot * _scaleUV.x, cosRot * _scaleUV.y);

	cosRot = cos(_rotateUV.y);
	sinRot = sin(_rotateUV.y);

	_uv2RotScale.set(cosRot * _scaleUV.z, -sinRot * _scaleUV.w, sinRot * _scaleUV.z, cosRot * _scaleUV.w);
}

C3DAnimatedLight* C3DAnimatedLight::create(const C3DVector3& color, C3DTexture* texture)
{
	C3DAnimatedLight* pLight = new C3DAnimatedLight();
	pLight->setColor(color);
	pLight->setTexture(texture);
	pLight->autorelease();
	return pLight;
}

C3DAnimatedLight* C3DAnimatedLight::create(const C3DVector3& color, const std::string& texturePath)
{
	C3DTexture* texture = C3DTexture::create(texturePath);
	C3DAnimatedLight* light = create(color, texture);
	//SAFE_RELEASE(texture);
	return light;
}

void C3DAnimatedLight::setIntensity(float orginalColor, float texture1, float texture2, float textureProduct)
{
	_intensity.set(orginalColor, texture1, texture2, textureProduct);
}

void C3DAnimatedLight::setOffsetUV(float u1, float v1, float u2, float v2)
{
	_offsetUV.set(u1, v1, u2, v2);
}

void C3DAnimatedLight::setRotateSpeedUV(float rotate1, float rotate2)
{
	_rotateSpeedUV.set(rotate1, rotate2);
}
}