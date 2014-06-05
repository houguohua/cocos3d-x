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

#include "C3DSubMesh.h"

namespace cocos3d
{
C3DSubMesh::C3DSubMesh() :
    _mesh(NULL), _meshIndex(0), _primitiveType(PrimitiveType_TRIANGLES), _indexCount(0), _indexBuffer(0), _dynamic(false)
{
}

C3DSubMesh::~C3DSubMesh()
{
    if (_indexBuffer)
    {
        glDeleteBuffers(1, &_indexBuffer);
    }
}

void C3DSubMesh::reset()
{
	_indexBuffer = 0;
}

C3DSubMesh* C3DSubMesh::create(C3DMesh* mesh, unsigned int meshIndex, PrimitiveType primitiveType,
    IndexFormat indexFormat, unsigned int indexCount, bool dynamic)
{
    // Create a VBO for our index buffer.
    GLuint vbo;
    GL_ASSERT( glGenBuffers(1, &vbo) );
    if (GL_LAST_ERROR())
    {
        return NULL;
    }

    GL_ASSERT( glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo) );
    if (GL_LAST_ERROR())
    {
        glDeleteBuffers(1, &vbo);
        return NULL;
    }

    unsigned int indexSize = 0;
    switch (indexFormat)
    {
    case IndexFormat_INDEX8:
        indexSize = 1;
        break;
    case IndexFormat_INDEX16:
        indexSize = 2;
        break;
    case IndexFormat_INDEX32:
        indexSize = 4;
        break;
    }
    GL_CHECK( glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexSize * indexCount, NULL, dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW) );
    if (GL_LAST_ERROR())
    {
        glDeleteBuffers(1, &vbo);
        return NULL;
    }
	 GL_ASSERT( glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0) );

    C3DSubMesh* part = new C3DSubMesh();
    part->_mesh = mesh;
    part->_meshIndex = meshIndex;
    part->_primitiveType = primitiveType;
    part->_indexFormat = indexFormat;
    part->_indexCount = indexCount;
    part->_indexBuffer = vbo;
    part->_dynamic = dynamic;

    return part;
}

unsigned int C3DSubMesh::getMeshIndex() const
{
    return _meshIndex;
}

PrimitiveType C3DSubMesh::getPrimitiveType() const
{
    return _primitiveType;
}

unsigned int C3DSubMesh::getIndexCount() const
{
    return _indexCount;
}

IndexFormat C3DSubMesh::getIndexFormat() const
{
    return _indexFormat;
}

IndexBufferHandle C3DSubMesh::getIndexBuffer() const
{
    return _indexBuffer;
}

bool C3DSubMesh::isDynamic() const
{
    return _dynamic;
}

void C3DSubMesh::setIndexData(void* indexData, unsigned int indexStart, unsigned int indexCount)
{
    GL_ASSERT( glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indexBuffer) );

    unsigned int indexSize = 0;
    switch (_indexFormat)
    {
    case IndexFormat_INDEX8:
        indexSize = 1;
        break;
    case IndexFormat_INDEX16:
        indexSize = 2;
        break;
    case IndexFormat_INDEX32:
        indexSize = 4;
        break;
    }

    if (indexStart == 0 && indexCount == 0)
    {
        GL_ASSERT( glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexSize * _indexCount, indexData, _dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW) );
    }
    else
    {
        if (indexCount == 0)
        {
            indexCount = _indexCount - indexStart;
        }

        GL_ASSERT( glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, indexStart * indexSize, indexCount * indexSize, indexData) );
    }

	GL_ASSERT( glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0) );
}
}