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

#ifndef SKINLESSMORPHMODEL_H_
#define SKINLESSMORPHMODEL_H_

#include "C3DSkinlessModel.h"

namespace cocos3d
{
class C3DMorph;

class C3DSkinlessMorphModel : public C3DSkinlessModel
{
public:

    C3DSkinlessMorphModel();

    ~C3DSkinlessMorphModel();

	static C3DSkinlessMorphModel* create();

	
	virtual C3DModel* clone(C3DNode::CloneContext& context) const;

	C3DMorph* getMorph();
	void setMorph(C3DMorph* morph);

	void pushMorph(int morphTargetIndex,float weight);
	void popMorph(int morphTargetIndex);
	void changeMorph(int morphTargetIndex,float weight);

protected:
	virtual void copyFrom(const C3DSkinlessMorphModel* other);

private:
	 C3DMorph* _morph;
};

}

#endif
