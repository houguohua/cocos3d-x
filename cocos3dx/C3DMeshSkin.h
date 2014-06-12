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

#ifndef MESHSKIN_H_
#define MESHSKIN_H_

#include "C3DMatrix.h"
#include "C3DTransform.h"
#include "C3DNode.h"

namespace cocos3d
{
class C3DResourceLoader;
class C3DModel;
class C3DBone;
class C3DNode;
class BonePart;

/**
 * Represents the skin for a mesh.
 */
class C3DMeshSkin : public C3DNode::Listener
{
    friend class C3DResourceLoader;
    friend class C3DSkinModel;
	friend class C3DSkinMorphModel;
    friend class C3DBone;
    friend class C3DNode;

public:

	 C3DMeshSkin();

    ~C3DMeshSkin();

    const C3DMatrix& getBindShape();

    void setBindShape(const float* matrix);

    unsigned int getJointCount();

    C3DBone* getJoint(unsigned int index);

    C3DBone* getJoint(const std::string& id);

    C3DBone* getRootJoint() const;

    void setRootJoint(C3DBone* joint);

    int getJointIndex(C3DBone* joint);

	void setBonePartIndex(unsigned int index);
	unsigned int getBonePartIndex();

    C3DVector4* getMatrixPalette(unsigned int index);

    unsigned int getMatrixPaletteSize(unsigned index);

    C3DModel* getModel() const;

    void transformChanged(C3DTransform* transform);

	BonePart* addPart(unsigned int batchID, unsigned int offsetVertexIndex, unsigned int numVertexIndex);

    void copyFrom(C3DMeshSkin* skin, C3DNode::CloneContext& context);

private:

    void setJointCount(unsigned int jointCount);

    void setJoint(C3DBone* joint, unsigned int index);

    void clearJoints();

    C3DMatrix _bindShape;
    std::vector<C3DBone*> _joints;
    C3DBone* _rootJoint;

    // Pointer to the array of palette matrices.
    // This array is passed to the vertex shader as a uniform.
    // Each 4x3 row-wise matrix is represented as 3 C3DVector4's.
    // The number of C3DVector4's is (_joints.size() * 3).
    C3DVector4* _matrixPalette;
    C3DModel* _model;

	unsigned int _partCount;
	BonePart** _parts;
	unsigned int _curPartIndex;
};
}

#endif
