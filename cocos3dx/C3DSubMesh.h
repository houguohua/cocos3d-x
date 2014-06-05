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

#ifndef SUBMESH_H_
#define SUBMESH_H_
#include "EnumDef_GL.h"

namespace cocos3d
{
class C3DMesh;

/**
 * Defines a part of a mesh describing the way the
 * mesh's vertices are connected together.
 */
class C3DSubMesh
{
    friend class C3DMesh;
	friend class C3DModel;
    friend class C3DSkinModel;
	friend class C3DSkinlessModel;
	
public:

    ~C3DSubMesh();
	void reset();

    unsigned int getMeshIndex() const;

    PrimitiveType getPrimitiveType() const;

    unsigned int getIndexCount() const;

    inline unsigned int getTriangleCount() const
    {
        if (_primitiveType == PrimitiveType_TRIANGLES)
            return _indexCount / 3;

        if (_primitiveType == PrimitiveType_TRIANGLE_STRIP)
            return _indexCount - 2;

        return 0;
    }

   IndexFormat getIndexFormat() const;

    IndexBufferHandle getIndexBuffer() const;

    bool isDynamic() const;//Determines if the indices are dynamic.

    void setIndexData(void* indexData, unsigned int indexStart, unsigned int indexCount);

private:

    C3DSubMesh();

    static C3DSubMesh* create(C3DMesh* mesh, unsigned int meshIndex, PrimitiveType primitiveType, IndexFormat indexFormat, unsigned int indexCount, bool dynamic = false);

    C3DMesh* _mesh;
    unsigned int _meshIndex;
    PrimitiveType _primitiveType;
    IndexFormat _indexFormat;
    unsigned int _indexCount;
    IndexBufferHandle _indexBuffer;
    bool _dynamic;
};
}

#endif
