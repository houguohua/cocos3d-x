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

#ifndef C3DSprite_H_
#define C3DSprite_H_
#include <list>
#include <vector>
#include <map>
#include <string>
#include "C3DNode.h"

#include "C3DRenderNode.h"
#include "C3DResource.h"
#include "cocos2d.h"

namespace cocos3d
{
class C3DVector2;
class C3DVector3;
class C3DScene;
class AttachNode;
class C3DNode;
class C3DActionListener;
class C3DBatchMesh;

class C3DAnimation;
class C3DAnimationClip;
class C3DCamera;
class C3DMaterial;
class C3DScene;
class C3DNode;
class C3DSprite;
class C3DSkeleton;
class C3DBone;

class C3DActionListener;

/**
*Defines this dynamic object of the scene,which can play animation,change material.
*/
class  C3DSprite : public C3DRenderNode
{
	friend class C3DScene;
	friend class C3DResourceLoader;
	friend class C3DRenderNodeManager;

public:

	C3DSprite(const std::string& id);
    ~C3DSprite(void);

	C3DNode::Type getType() const;

	virtual bool load(C3DResourceLoader* loader, bool isLoadAll = false);

	void loadNode(const std::string& nodeName);
	void loadNodes(std::list<std::string>* models);

	/**
    * Update for handling update routines.
    *
    * @param elapsedTime The elapsed game time.
    */
	void update(long elapsedTime);

	/**
	* draw debug info
	*/
	virtual void drawDebug();

	/**
    * load the model animation of the avatar.
    */
	C3DAnimation* createAnimation();

	/**
    * create the facial anim of the avatar model.
    */
	//void createFacialAnim();

	/**
    * add clip info of anim.
    *
    * @param name The clip name.
	* @param startFrame The startFrame.
	* @param endFrame The endFrame.
	* @param repeatCount The play counter.
	* @param speed The play speed.
    */
	C3DAnimationClip* addAnimationClip(const std::string& name,unsigned int startFrame,unsigned int endFrame,float repeatCount,float speed);

	// for editor
	bool replaceAnimationClip(const std::string& name,unsigned int startFrame,unsigned int endFrame,float repeatCount,float speed);

	/**
    * play this avatar model's action by the given name
    *
    * @param animName The action name.
    */
	void playAnimationClip(const std::string& name);
	void stopAnimationClip(const std::string& name);
    void pauseAnimationClip(const std::string& name);
    void resumeAnimationClip(const std::string& name);
    bool isAnimationClipPlaying(const std::string& name);

	bool loadMesh(const std::string& meshName);

	virtual void calculateBoundingBox_();

	void setSkeleton(C3DBone* joint);

	C3DBone* getSkeletonRootBone()const;

	C3DBone* getBone(const std::string& name)const;

    C3DAnimationClip* getAnimationClip(const std::string& strClip);
    C3DAnimationClip* getCurAnimationClip();

	int getAnimationFrameCount();

	virtual C3DNode* clone(CloneContext& context) const;

	void stopAllAnimationClip();

    C3DAnimation* getAnimation() { return _animation; }
protected:
	// create sprite and add it to autorelease pool
	static C3DSprite* create(const std::string& id);

	virtual void copyFrom(const C3DTransform* other, C3DNode::CloneContext& context);

private:

	C3DSkeleton* _skeleton;
	C3DAnimation* _animation;
};
}

#endif
