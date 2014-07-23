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

#ifndef MESH_H_
#define MESH_H_

#include "C3DBaseMesh.h"

namespace cocos3d
{
class C3DVertexFormat;
class C3DSubMesh;
class C3DAABB;

/**
 * Defines a mesh supporting various vertex formats and 1 or more
 * C3DSubMesh(s) to define how the vertices are connected.
 */
class C3DMesh : public C3DBaseMesh
{
    friend class C3DModel;
    friend class C3DResourceLoader;
	friend class Geo;

public:

	/**
	create mesh
	@param vertexFormat defines vertex format
	@param vertexCount vertex count
	@param dynamic dynamic increase the vertex number or not
	*/
    static  C3DMesh* create(C3DVertexFormat* vertexFormat, unsigned int vertexCount, bool dynamic = false);

    const std::string& getUrl() const;

    unsigned int getVertexCount() const;

    VertexBufferHandle getVertexBuffer() const;

    bool isDynamic() const;

    virtual void setVertexData(void* vertexData, unsigned int vertexStart = 0, unsigned int vertexCount = 0);

    C3DSubMesh* addSubMesh(PrimitiveType primitiveType, IndexFormat indexFormat, unsigned int indexCount, bool dynamic = false);

    unsigned int getSubMeshCount() const;

    C3DSubMesh* getSubMesh(unsigned int index);

    virtual unsigned int getTriangleCount() const;

    virtual ~C3DMesh();

	void reset();

	C3DAABB* getBoundingBox() { return _boundingBox; };

protected:
	C3DMesh(C3DVertexFormat* vertexFormat,PrimitiveType primitiveType = PrimitiveType_TRIANGLES);
	bool init(C3DVertexFormat* vertexFormat, unsigned int vertexCount, bool dynamic);
	virtual void reload();

protected:

    std::string _url;
    unsigned int _vertexCount;
    VertexBufferHandle _vertexBuffer;
    unsigned int _subMeshCount;
    C3DSubMesh** _subMeshs;
    bool _dynamic;
	C3DAABB* _boundingBox;
};

}
#endif
