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

#ifndef POSTPROCESS_H
#define POSTPROCESS_H

#include "C3DNode.h"
#include "C3DVector2.h"
#include "C3DPostEffect.h"

namespace cocos3d
{
class C3DFrameBuffer;
class C3DTexture;
class C3DSampler;
class C3DMaterial;
class C3DModel;
class C3DPostEffect;
class C3DPass;

extern C3DPostProcess* g_pPostProcess;
/**
Post process
A C3DPostProcess can have serial post effect, but with only one active right now
*/

class C3DPostProcess : public C3DNode
{
public:

public:
    /**
     * create post process
     */
    static C3DPostProcess* create(const std::string& id, unsigned int texWidth, unsigned int texHeight/*, const std::string& techniqueId*/);

	/**
     * get type inherit from C3DNode
     */
	virtual C3DNode::Type getType() const { return C3DNode::NodeType_PostProcess; };
	/**
     * get frame buffer
     */
	C3DFrameBuffer* getFrameBuffer() const;

	/**
     * begin & end draw
     */
    virtual void beginDraw();
    virtual void endDraw();

	void preChannelDraw();
	void postChannelDraw();

	/**
     * draw routine
     */
	virtual void draw();

    virtual void transformChanged();

	bool addPostEffect( C3DPostEffect* effect );
	void removePostEffect( const std::string& name );
	C3DPostEffect* getPostEffect( const std::string& name );

	/**
     * set active post effect by type
     */
    C3DPostEffect* setActiveEffect( const std::string& name );

	/**
     * get frame buffer width
     */
	unsigned int getFBWidth();
	/**
     * get frame buffer height
     */
	unsigned int getFBHeight();

    //C3DModel* getModel()
    //{
    //    return _model;
    //}

    /**
     * get frame buffer sampler
     */
    C3DSampler* getFramebufferSampler()
    {
        return _sampler;
    }

    C3DPostEffect* getActiveEffect()
    {
        return _curPostEffect;
    }

	std::vector<std::string> getEffectNames(void) const;

	void reload();

public:
	typedef std::map<std::string, C3DPostEffect*> PostEffects;
protected:
	/**
     * constructor & destructor
     */
	C3DPostProcess();
	C3DPostProcess(const std::string& id,C3DFrameBuffer* buffer);
    virtual ~C3DPostProcess();

	bool _isDraw;

	// properties
    C3DFrameBuffer* _framebuffer;
    C3DSampler* _sampler;//frame buffer sampler

	//C3DModel* _model;

	PostEffects _postEffects;

	C3DPostEffect* _curPostEffect;

	unsigned int _fbWidth;
	unsigned int _fbHeight;
};

}

#endif
