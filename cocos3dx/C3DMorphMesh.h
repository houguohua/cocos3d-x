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

#ifndef MORPHMESH_H_
#define MORPHMESH_H_

#include "C3DVector3.h"
#include "Base.h"
#include "EnumDef_GL.h"
#include "cocos2d.h"
#include "C3DMesh.h"

namespace cocos3d
{
class C3DVertexFormat;
class C3DMorph;

/**
 * Defines a mesh supporting morph deform.
 */
class C3DMorphMesh : public C3DMesh
{
    friend class C3DModel;
    friend class C3DResourceLoader;
	friend class Geo;

public:
	static C3DMorphMesh* create(C3DVertexFormat* vertexFormat, unsigned int vertexCount, bool dynamic);
    virtual void setVertexData(void* vertexData, unsigned int vertexStart = 0, unsigned int vertexCount = 0);

	void pushMorph(C3DMorph* morph,int morphTargetIndex,float weight);
	void popMorph(C3DMorph* morph,int morphTargetIndex);
	void changeMorph(C3DMorph* morph,int morphTargetIndex,float weight);
	void clearMorph(C3DMorph* morph);

    virtual ~C3DMorphMesh();
protected:
	virtual void reload();
private:
	void setMorphVertexData(void* vertexData);
	C3DMorphMesh(C3DVertexFormat* vertexFormat,PrimitiveType primitiveType = PrimitiveType_TRIANGLES);
	void applyMorph(C3DMorph* morph);

private:

	unsigned char* _vertexData;
};
}

#endif
