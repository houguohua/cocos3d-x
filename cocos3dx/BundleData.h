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

#ifndef BUNDLEDATA_H_
#define BUNDLEDATA_H_

#include <vector>
#include "C3DMatrix.h"
#include "EnumDef_GL.h"

namespace cocos3d
{
class C3DMeshSkin;
class SubMeshData;
class C3DVertexFormat;
class C3DVertexElement;
class C3DAABB;

/**
mesh data
*/
class MeshData
{
public:
	/**
     * Constructor & Destructor.
     */
	MeshData(C3DVertexElement* elements, unsigned int elementCount);
    ~MeshData();

    C3DVertexFormat* vertexFormat;
    unsigned int vertexCount;
    unsigned char* vertexData;
	C3DAABB* boundingBox;

    PrimitiveType primitiveType;
    std::vector<SubMeshData*> parts;
};

//class MorphTargetData
//{
//public:
//	MorphTargetData();
//	~MorphTargetData();
//	unsigned int index;
//	std::string name;
//	std::vector<VertexOffset> offsets;
//};
//
//class MorphData
//{
//public:
//	MorphData();
//	~MorphData();
//
//	std::vector<MorphTargetData*>* _morphTargets;
//
//};

/**
bone index data
*/
class BonePartData
{
public:
	/**
     * Constructor & Destructor.
     */
	BonePartData();
	~BonePartData();

	unsigned int _batchID;
	unsigned int _offsetVertexIndex;
	unsigned int _numVertexIndex;

    unsigned int indexCount;
    unsigned char* indexData;
};

/**
mesh skin data
*/
class MeshSkinData
{
public:
	/**
     * Constructor & Destructor.
     */
	MeshSkinData();
	~MeshSkinData();

    C3DMeshSkin* skin;
    std::vector<std::string> joints;
    std::vector<C3DMatrix> inverseBindPoseMatrices;
};

/**
Mesh part index data
*/
class SubMeshData
{
public:
	/**
     * Constructor & Destructor.
     */
    SubMeshData();
    ~SubMeshData();

    PrimitiveType primitiveType;
    IndexFormat indexFormat;
    unsigned int indexCount;
    unsigned char* indexData;
};
}

#endif
