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

#ifndef BATCHMESH_H_
#define BATCHMESH_H_

#include "EnumDef_GL.h"
#include "C3DBaseMesh.h"

namespace cocos3d
{
class C3DVertexFormat;

/**
 * Defines a class for rendering multiple mesh into a single draw call.
 */
class C3DBatchMesh : public C3DBaseMesh
{
friend class C3DBatchModel;
public:
	C3DBatchMesh(C3DVertexFormat* vertexFormat);
	C3DBatchMesh(C3DVertexFormat* vertexFormat, PrimitiveType primitiveType, bool bIndex = false, unsigned int initialCapacity = 128, unsigned int growSize = 128);

    ~C3DBatchMesh();

    void init();
    
	unsigned int getVertexCapacity() const
	{
		return _vertexCapacity;
	}
	void setVertexCapacity(unsigned int capacity);

	unsigned int getIndexCapacity() const
	{
		return _indexCapacity;
	}
    void setIndexCapacity(unsigned int capacity);

	// adds a group of primitives to the batch.
	void add(const unsigned char* vertices, unsigned int vertexCount);
	void add(const unsigned char* vertices, unsigned int vertexCount, const unsigned short* indices, unsigned int indexCount);

	void addVertex( const unsigned char* vertices, unsigned int vertexCount );
	void addIndex( const unsigned short* indices, unsigned int indexCount, unsigned int vertexOffset );

	void clear();
	void draw();
	unsigned char* getVertices(void) const
	{
		return _vertices;
	}
	unsigned short* getIndices(void) const
	{
		return _indices;
	}

private:

    bool resizeVertex(unsigned int capacity);
    bool resizeIndex(unsigned int capacity);

	//const C3DVertexFormat* _vertexFormat;
   // PrimitiveType _primitiveType;

    unsigned int _growSize;//the value the batch grow when it overflows.
    unsigned int _vertexCapacity;//the  capacity of the vertex.
    unsigned int _vertexCount;//cur number of vertices
    unsigned char* _vertices;//vertice array
    unsigned char* _verticesPtr;//cur vertice offset pointer

    bool _bUseIndex;
    unsigned int _indexCapacity;
    unsigned short* _indices;
    unsigned short* _indicesPtr;
    unsigned int _indexCount;
};
}

#endif
