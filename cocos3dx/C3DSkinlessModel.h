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

#ifndef NODELESSMODEL_H_
#define NODELESSMODEL_H_

#include "C3DModel.h"
#include "C3DNode.h"

namespace cocos3d
{
/**
 * Defines a model which doesn't have skin mesh.
 */
class C3DSkinlessModel : public C3DModel
{
public:

    C3DSkinlessModel();

    ~C3DSkinlessModel();

	static C3DModel* create();

	//void draw();

	virtual std::string getDefaultMaterialName();

	virtual C3DModel* clone(C3DNode::CloneContext& context) const;

	/**
     * Draws this mesh instance.
     *
     * This method binds the vertex buffer and index buffers for the C3DMesh and
     * all of its C3DSubMeshs and draws the mesh geometry. Any other state
     * neccessary to render the C3DMesh, such as rendering states, shader state,
     * and so on, should be set up before calling this method.
     *
    */
	virtual void draw(void);


};

}

#endif
