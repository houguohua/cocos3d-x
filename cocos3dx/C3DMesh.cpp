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
#include "C3DMesh.h"
#include "C3DSubMesh.h"
#include "C3DVertexFormat.h"
#include "C3DAABB.h"

namespace cocos3d
{
C3DMesh::C3DMesh(C3DVertexFormat* vertexFormat, PrimitiveType primitiveType)
    :C3DBaseMesh(vertexFormat, primitiveType),
	 _vertexCount(0), 
	 _vertexBuffer(0),
	 _subMeshCount(0),
	 _subMeshs(NULL),
	 _dynamic(false),
	 _boundingBox(NULL)
{
	//LOG_TRACE_VARG("%p +C3DMesh", this);
}

C3DMesh::~C3DMesh()
{
	//LOG_TRACE_VARG("%p -C3DMesh", this);
	for (unsigned int i = 0; i < _subMeshCount; ++i)
	{
		SAFE_DELETE(_subMeshs[i]);
	}
	SAFE_DELETE_ARRAY(_subMeshs);

	if (_vertexBuffer)
	{
		glDeleteBuffers(1, &_vertexBuffer);
		_vertexBuffer = 0;
	}

	SAFE_DELETE(_boundingBox);
}

void C3DMesh::reload()
{
	for (unsigned int i = 0; i < _subMeshCount; ++i)
    {
		_subMeshs[i]->reset();
        SAFE_DELETE(_subMeshs[i]);
    }

	_subMeshCount = 0;
    SAFE_DELETE_ARRAY(_subMeshs);

    _vertexBuffer = 0;
	
	_url = std::string("");

	SAFE_DELETE(_boundingBox);
}

bool C3DMesh::init(C3DVertexFormat* vertexFormat, unsigned int vertexCount, bool dynamic)
{
	GLuint vbo;
    GL_ASSERT( glGenBuffers(1, &vbo) );
    if (GL_LAST_ERROR())
    {
        return false;
    }

    GL_ASSERT( glBindBuffer(GL_ARRAY_BUFFER, vbo) );
    if (GL_LAST_ERROR())
    {
        glDeleteBuffers(1, &vbo);
        return false;
    }

    GL_CHECK( glBufferData(GL_ARRAY_BUFFER, vertexFormat->getVertexSize() * vertexCount, NULL, dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW) );
    if (GL_LAST_ERROR())
    {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glDeleteBuffers(1, &vbo);
        return false;
    }

	GL_ASSERT( glBindBuffer(GL_ARRAY_BUFFER, 0) );

	_vertexCount	= vertexCount;
	_vertexBuffer	= vbo;
	_dynamic		= dynamic;
	_boundingBox	= new C3DAABB();

	return  true;
}

C3DMesh* C3DMesh::create(C3DVertexFormat* vertexFormat, unsigned int vertexCount, bool dynamic)
{
    C3DMesh* mesh = new C3DMesh(vertexFormat);
	mesh->autorelease();

	bool res = mesh->init(vertexFormat, vertexCount, dynamic);
	if(res == true)
		return mesh;
	else
	{
		return NULL;
	}
}

const std::string& C3DMesh::getUrl() const
{
    return _url;
}

unsigned int C3DMesh::getVertexCount() const
{
    return _vertexCount;
}

VertexBufferHandle C3DMesh::getVertexBuffer() const
{
    return _vertexBuffer;
}

bool C3DMesh::isDynamic() const
{
    return _dynamic;
}

void C3DMesh::setVertexData(void* vertexData, unsigned int vertexStart, unsigned int vertexCount)
{
    GL_ASSERT( glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer) );

    if (vertexStart == 0 && vertexCount == 0)
    {
        GL_ASSERT( glBufferData(GL_ARRAY_BUFFER, _vertexCount * _vertexFormat->getVertexSize(), vertexData, _dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW) );
    }
    else
    {
        if (vertexCount == 0)
        {
            vertexCount = _vertexCount - vertexStart;
        }

        GL_ASSERT( glBufferSubData(GL_ARRAY_BUFFER, vertexStart * _vertexFormat->getVertexSize(), vertexCount * _vertexFormat->getVertexSize(), vertexData) );
    }

	GL_ASSERT( glBindBuffer(GL_ARRAY_BUFFER, 0) );
}

unsigned int C3DMesh::getTriangleCount() const
{
    int nTriangle = 0;
    if (_subMeshCount > 0)
    {
        for (unsigned int i = 0; i < _subMeshCount; i++) {
            nTriangle += _subMeshs[i]->getTriangleCount();
        }
    }
    else
    {
        if (_primitiveType == PrimitiveType_TRIANGLES)
            nTriangle += _vertexCount / 3;
        else if (_primitiveType == PrimitiveType_TRIANGLE_STRIP)
            nTriangle += _vertexCount - 2;
    }
    return nTriangle;
}

C3DSubMesh* C3DMesh::addSubMesh(PrimitiveType primitiveType, IndexFormat indexFormat, unsigned int indexCount, bool dynamic)
{
    C3DSubMesh* part = C3DSubMesh::create(this, _subMeshCount, primitiveType, indexFormat, indexCount, dynamic);
    if (part)
    {
        // Increase size of part array and copy old subets into it.
        C3DSubMesh** olds = _subMeshs;
        _subMeshs = new C3DSubMesh*[_subMeshCount + 1];
        for (unsigned int i = 0; i < _subMeshCount; ++i)
        {
            _subMeshs[i] = olds[i];
        }

        // Add new part to array.
        _subMeshs[_subMeshCount++] = part;

        // Delete old part array.
        SAFE_DELETE_ARRAY(olds);
    }

    return part;
}

unsigned int C3DMesh::getSubMeshCount() const
{
    return _subMeshCount;
}

C3DSubMesh* C3DMesh::getSubMesh(unsigned int index)
{
    return _subMeshs[index];
}

}