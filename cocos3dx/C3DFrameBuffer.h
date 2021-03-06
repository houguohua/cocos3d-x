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

#ifndef FRAMEBUFFER_H_
#define FRAMEBUFFER_H_

#include "Base.h"

#include "cocos2d.h"
#include "C3DViewport.h"

namespace cocos3d
{
class C3DRenderTarget;
class C3DDepthStencilTarget;

/**
 * Defines a frame buffer object that may contain one or more render targets and optionally
 * a depth-stencil target.
 *
 * Frame buffers can be created and used for off-screen rendering, which is useful for
 * techniques such as shadow mapping and post-processing.
 *
 * When binding a custom frame buffer, you should always store the return value of
 * C3DFrameBuffer::bind and restore it when you are finished drawing to your frame buffer.
 *
 * To bind the default frame buffer, call C3DFrameBuffer::bindDefault.
 */
class C3DFrameBuffer : public cocos2d::CCObject
{
public:

    /**
     * Creates a new C3DFrameBuffer with a C3DRenderTarget of the specified width and height.
     */
	static C3DFrameBuffer* create(const std::string& id, unsigned int width, unsigned int height, unsigned int fmtColor = GL_RGBA, unsigned int fmtDepth = 0);
    static C3DFrameBuffer* getFrameBuffer(const std::string& id);

	/**
     * string id
     */
    const std::string& getID() const;

	/**
     * get & set render target.
     */
    bool setRenderTarget(C3DRenderTarget* target);
    C3DRenderTarget* getRenderTarget() const;

	/**
     * get & set DepthStencilTarget
     */
    bool setDepthStencilTarget(C3DDepthStencilTarget* target);
    C3DDepthStencilTarget* getDepthStencilTarget() const;

	/**
     * bind this frame buffer and render to it.
     */
    void bind();

	/**
     * bind default and render to display.
     */
    static void bindDefault(); //rendering to the display

	/**
     * restore to frame buffer before bind.
     */
    void unbind(); //restore framebuffer

	/**
     * get fbo handle.
     */
    FrameBufferHandle getFBO()
	{
		return _handle;
	}

    unsigned int getWidth() const
    {
        return _width;
    }

    unsigned int getHeight() const
    {
        return  _height;
    }

	void reload();

private:

	/**
     * Constructor & Destructor.
     */
    C3DFrameBuffer(const std::string& id);
    ~C3DFrameBuffer();

	// properties
    std::string _id;
    FrameBufferHandle _handle;
    C3DRenderTarget* _renderTarget;
    C3DDepthStencilTarget* _depthStencilTarget;

    unsigned int _width;
    unsigned int _height;
    bool _isBind;
    C3DViewport _oldViewport;
    GLint _oldFBO;
};

class C3DFrameBufferMgr : public cocos2d::Ref
{
public:
	typedef std::vector<C3DFrameBuffer*> T_CACHE_CONTAINER;
public:
	static C3DFrameBufferMgr* getInstance();

	void add(C3DFrameBuffer* texture);
	void remove(C3DFrameBuffer* texture);
	C3DFrameBuffer* get(const std::string& strID);

	void reload();
protected:
	C3DFrameBufferMgr();
	~C3DFrameBufferMgr();
protected:
	T_CACHE_CONTAINER _frameBufs;
};}

#endif
