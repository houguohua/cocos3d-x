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

#include "BundleData.h"
#include "C3DVertexFormat.h"
#include "C3DAABB.h"

namespace cocos3d
{
MeshData::MeshData(C3DVertexElement* elements, unsigned int elementCount)
	: vertexFormat(NULL), vertexCount(0), vertexData(NULL), boundingBox(NULL)
{
	vertexFormat = new C3DVertexFormat(elements,elementCount);
	boundingBox = new C3DAABB();
}

MeshData::~MeshData()
{
	SAFE_DELETE(vertexFormat);
    SAFE_DELETE_ARRAY(vertexData);

    for (unsigned int i = 0; i < parts.size(); ++i)
    {
        SAFE_DELETE(parts[i]);
    }
	SAFE_DELETE(boundingBox);
}

MeshSkinData::MeshSkinData()
{
	skin = NULL;
}

MeshSkinData::~MeshSkinData()
{
	skin = NULL;
}

BonePartData::BonePartData()
{
	indexData = NULL;
}

BonePartData::~BonePartData()
{
	SAFE_DELETE_ARRAY(indexData);
}

SubMeshData::SubMeshData() :
    indexCount(0), indexData(NULL)
{
}

SubMeshData::~SubMeshData()
{
    SAFE_DELETE_ARRAY(indexData);
}
}