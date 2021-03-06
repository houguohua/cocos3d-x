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
#include "MaterialParameter.h"
#include "C3DEffect.h"
#include "C3DSampler.h"
#include "C3DSamplerCube.h"

namespace cocos3d
{
MaterialParameter::MaterialParameter(const std::string& name) :
    _type(MaterialParameter::NONE), _count(1), _dynamic(false), _name(name), _uniform(NULL)
{
    clearValue();
}

MaterialParameter::~MaterialParameter()
{
    clearValue();
}

void MaterialParameter::clearValue()
{
    if (_dynamic)
    {
        switch (_type)
        {
        case MaterialParameter::FLOAT:
        case MaterialParameter::VECTOR2:
        case MaterialParameter::VECTOR3:
        case MaterialParameter::VECTOR4:
        case MaterialParameter::MATRIX:
            SAFE_DELETE_ARRAY(_value.floatPtrValue);
            break;
        case MaterialParameter::INT:
            SAFE_DELETE_ARRAY(_value.intPtrValue);
            break;
        case MaterialParameter::METHOD:
		case MaterialParameter::METHOD_PARAM:
            SAFE_RELEASE(_value.method);
            break;
        default:
            break;
        }

        _dynamic = false;
        _count = 1;
    }
    else
    {
        switch (_type)
        {
        case MaterialParameter::SAMPLER:
            if (_value.samplerValue)
            {
                const_cast<C3DSampler*>(_value.samplerValue)->release();
            }
            break;

		case MaterialParameter::SAMPLERCUBE:
			if (_value.samplerValue)
			{
				const_cast<C3DSamplerCube*>(_value.samplerCubeValue)->release();
			}
			break;
        default:
            {
                
            }break;
        }
    }

    memset(&_value, 0, sizeof(_value));
    _type = MaterialParameter::NONE;
}

const std::string& MaterialParameter::getName() const
{
    return _name;
}

void MaterialParameter::setValue(float value)
{
    clearValue();

    _value.floatValue = value;
    _type = MaterialParameter::FLOAT;
}

void MaterialParameter::setValue(int value)
{
    clearValue();

    _value.intValue = value;
    _type = MaterialParameter::INT;
}

void MaterialParameter::setValue(const float* values, unsigned int count)
{
    clearValue();

    _value.floatPtrValue = const_cast<float*> (values);
    _count = count;
    _type = MaterialParameter::FLOAT;
}

void MaterialParameter::setValue(const int* values, unsigned int count)
{
    clearValue();

    _value.intPtrValue = const_cast<int*> (values);
    _count = count;
    _type = MaterialParameter::INT;
}

void MaterialParameter::setValue(const C3DVector2& value)
{
    clearValue();

    // Copy data by-value into a dynamic array.
    float* array = new float[2];
    memcpy(array, &value.x, sizeof(float) * 2);

    _value.floatPtrValue = array;
    _dynamic = true;
    _count = 1;
    _type = MaterialParameter::VECTOR2;
}

void MaterialParameter::setValue(const C3DVector2* values, unsigned int count)
{
    clearValue();

    _value.floatPtrValue = const_cast<float*> (&values[0].x);
    _count = count;
    _type = MaterialParameter::VECTOR2;
}

void MaterialParameter::setValue(const C3DVector3& value)
{
    clearValue();

    // Copy data by-value into a dynamic array.
    float* array = new float[3];
    memcpy(array, &value.x, sizeof(float) * 3);

    _value.floatPtrValue = array;
    _dynamic = true;
    _count = 1;
    _type = MaterialParameter::VECTOR3;
}

void MaterialParameter::setValue(const C3DVector3* values, unsigned int count)
{
    clearValue();

    _value.floatPtrValue = const_cast<float*> (&values[0].x);
    _count = count;
    _type = MaterialParameter::VECTOR3;
}

void MaterialParameter::setValue(const C3DVector4& value)
{
    clearValue();

    // Copy data by-value into a dynamic array.
    float* array = new float[4];
    memcpy(array, &value.x, sizeof(float) * 4);

    _value.floatPtrValue = array;
    _dynamic = true;
    _count = 1;
    _type = MaterialParameter::VECTOR4;
}

void MaterialParameter::setValue(const C3DVector4* values, unsigned int count)
{
    clearValue();

    _value.floatPtrValue = const_cast<float*> (&values[0].x);
    _count = count;
    _type = MaterialParameter::VECTOR4;
}

void MaterialParameter::setValue(const C3DMatrix& value)
{
    // If this parameter is already storing a single dynamic matrix, no need to clear it.
    if (!(_dynamic && _count == 1 && _type == MaterialParameter::MATRIX && _value.floatPtrValue != NULL))
    {
        clearValue();

        // Allocate a new dynamic matrix.
        _value.floatPtrValue = new float[16];
    }

    memcpy(_value.floatPtrValue, value.m, sizeof(float) * 16);

    _dynamic = true;
    _count = 1;
    _type = MaterialParameter::MATRIX;
}

void MaterialParameter::setValue(const C3DMatrix* values, unsigned int count)
{
    clearValue();

    _value.floatPtrValue = const_cast<C3DMatrix&> (values[0]).m;
    _count = count;
    _type = MaterialParameter::MATRIX;
}

void MaterialParameter::setValue(const C3DSampler* sampler)
{
    clearValue();

    if (sampler)
    {
       const_cast<C3DSampler*>(sampler)->retain();
        _value.samplerValue = sampler;
        _type = MaterialParameter::SAMPLER;
    }
}

void MaterialParameter::setValue(const C3DSamplerCube* sampler)
{
	clearValue();

	if (sampler)
	{
		const_cast<C3DSamplerCube*>(sampler)->retain();
		_value.samplerCubeValue = sampler;
		_type = MaterialParameter::SAMPLERCUBE;
	}
}

void MaterialParameter::setValue(const C3DTexture* texture)
{
    clearValue();

    if (texture)
    {
		_value.textureValue = texture;
        _type = MaterialParameter::TEXTURE;
    }
}

C3DSampler* MaterialParameter::setValue(const std::string& texturePath, bool generateMipmaps)
{
    if (!texturePath.empty())
    {
        clearValue();

        C3DSampler* sampler = C3DSampler::create(texturePath, generateMipmaps);
        if (sampler)
        {
            _value.samplerValue = sampler;
            _type = MaterialParameter::SAMPLER;
        }
        return sampler;
    }

    return NULL;
}

void MaterialParameter::bind(C3DEffect* effect)
{
    // If we had a Uniform cached that is not from the passed in effect,
    // we need to update our uniform to point to the new effect's uniform.
    if (!_uniform || _uniform->getEffect() != effect)
    {
        _uniform = effect->getUniform(_name);

        if (!_uniform)
        {
            // This parameter was not found in the specified effect, so do nothing.
      //      WARN_VARG("Warning: C3DMaterial parameter '%s' not found in effect '%s'.", _name.c_str(), effect->getId());
            return;
        }
    }

    switch (_type)
    {
    case MaterialParameter::FLOAT:
        if (_count == 1)
        {
            effect->bindValue(_uniform, _value.floatValue);
        }
        else
        {
            assert(_value.floatPtrValue);
            effect->bindValue(_uniform, _value.floatPtrValue, _count);
        }
        break;
    case MaterialParameter::INT:
        if (_count == 1)
        {
            effect->bindValue(_uniform, _value.intValue);
        }
        else
        {
            assert(_value.intPtrValue);
            effect->bindValue(_uniform, _value.intPtrValue, _count);
        }
        break;
    case MaterialParameter::VECTOR2:
        assert(_value.floatPtrValue);
        effect->bindValue(_uniform, reinterpret_cast<C3DVector2*>(_value.floatPtrValue), _count);
        break;
    case MaterialParameter::VECTOR3:
        assert(_value.floatPtrValue);
        effect->bindValue(_uniform, reinterpret_cast<C3DVector3*>(_value.floatPtrValue), _count);
        break;
    case MaterialParameter::VECTOR4:
        assert(_value.floatPtrValue);
        effect->bindValue(_uniform, reinterpret_cast<C3DVector4*>(_value.floatPtrValue), _count);
        break;
    case MaterialParameter::MATRIX:
        assert(_value.floatPtrValue);
        effect->bindValue(_uniform, reinterpret_cast<C3DMatrix*>(_value.floatPtrValue), _count);
        break;
    case MaterialParameter::SAMPLER:
        assert(_value.samplerValue);
        effect->bindValue(_uniform, _value.samplerValue);
        break;
	case MaterialParameter::SAMPLERCUBE:
		assert(_value.samplerCubeValue);
		effect->bindValue(_uniform, _value.samplerCubeValue);
		break;
	case MaterialParameter::TEXTURE:
        assert(_value.textureValue);
        effect->bindValue(_uniform, _value.textureValue);
        break;
    case MaterialParameter::METHOD:
        assert(_value.method);
        _value.method->bindValue(effect);
        break;
    default:
        break;
    }
}

void MaterialParameter::reload()
{
	_uniform = NULL;

	switch (_type)
	{
	case cocos3d::MaterialParameter::SAMPLER:
		{
			const_cast<C3DSampler*>(_value.samplerValue)->reload();
		}
		break;
	case cocos3d::MaterialParameter::SAMPLERCUBE:
		break;
	case cocos3d::MaterialParameter::TEXTURE:
		break;
	default:
		break;
	}
}

void MaterialParameter::setParamMethonAutoUniform(C3DEffect* effect)
{
    // If we had a Uniform cached that is not from the passed in effect,
    // we need to update our uniform to point to the new effect's uniform.
    if (!_uniform || _uniform->getEffect() != effect)
    {
        _uniform = effect->getUniform(_name);

        if (!_uniform)
        {
            // This parameter was not found in the specified effect, so do nothing.
          //  WARN_VARG("Warning: C3DMaterial parameter '%s' not found in effect '%s'.", _name.c_str(), effect->getId());
            return;
        }
    }

    switch (_type)
    {
    case MaterialParameter::METHOD_PARAM:
        assert(_value.method);
        _value.method->bindValue(effect);
        break;
    default:
        break;
    }
}

Uniform* MaterialParameter::getUniform()
{
	return _uniform;
}

MaterialParameter* MaterialParameter::clone() const
{
	MaterialParameter* other = new MaterialParameter(_name);
	other->_uniform = _uniform;
	other->_count = _count;
	other->_dynamic = _dynamic;
	other->_type = _type;
	memcpy(&other->_value, &_value, sizeof(_value));

	if (_dynamic)
	{
		switch (_type)
		{
		case NONE:
			break;
		case FLOAT:
			other->_value.floatPtrValue = new float[_count];
			memcpy(other->_value.floatPtrValue, _value.floatPtrValue, sizeof(float) * _count);
			break;
		case INT:
			other->_value.intPtrValue = new int[_count];
			memcpy(other->_value.intPtrValue, _value.intPtrValue, sizeof(int) * _count);
			break;
		case VECTOR2:
			other->_value.floatPtrValue = new float[_count * 2];
			memcpy(other->_value.floatPtrValue, _value.floatPtrValue, sizeof(float) * _count * 2);
			break;
		case VECTOR3:
			other->_value.floatPtrValue = new float[_count * 3];
			memcpy(other->_value.floatPtrValue, _value.floatPtrValue, sizeof(float) * _count * 3);
			break;
		case VECTOR4:
			other->_value.floatPtrValue = new float[_count * 4];
			memcpy(other->_value.floatPtrValue, _value.floatPtrValue, sizeof(float) * _count * 4);
			break;
		case MATRIX:
			other->_value.floatPtrValue = new float[_count * 16];
			memcpy(other->_value.floatPtrValue, _value.floatPtrValue, sizeof(float) * _count * 16);
			break;
		case SAMPLER:
		case TEXTURE:
			assert(false); // impossible
			break;
		case METHOD:
		case METHOD_PARAM:
			other->_value.method = _value.method->clone();
			break;
        default:
            break;
		}
	}
	else
	{
		switch (_type)
		{
		case SAMPLER:
			if (_value.samplerValue)
			{
				const_cast<C3DSampler*>(_value.samplerValue)->retain();
			}
			break;
		case SAMPLERCUBE:
			if (_value.samplerCubeValue)
			{
				const_cast<C3DSamplerCube*>(_value.samplerCubeValue)->retain();
			}
			break;
        default:
            break;
		}
	}
	return other;
}
}