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

#ifndef C3DSKINMODEL_H_
#define C3DSKINMODEL_H_

#include "C3DModel.h"
#include "C3DNode.h"

namespace cocos3d
{
class C3DMeshSkin;

/**
 * Defines a model which have skin mesh.
 */
class C3DSkinModel : public C3DModel
{
public:
	C3DMeshSkin* getSkin();
	void setSkin(C3DMeshSkin* skin);

    C3DSkinModel();

    ~C3DSkinModel();

	static C3DModel* create();

	void draw(void);

    virtual C3DModel* clone(C3DNode::CloneContext& context) const;

	virtual std::string getDefaultMaterialName();

protected:
	void copyFrom(const C3DSkinModel* other);
protected:
	 C3DMeshSkin* _skin;
};
}

#endif
