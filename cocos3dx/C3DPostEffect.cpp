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

#include "C3DPostEffect.h"
#include "C3DPostProcess.h"

#include "C3DMaterial.h"
#include "C3DRenderState.h"
#include "C3DTechnique.h"
#include "C3DPass.h"
#include "MaterialParameter.h"

#include "Base.h"
#include "C3DVector3.h"
#include "C3DTexture.h"
#include "C3DSampler.h"
#include "C3DFrameBuffer.h"
#include "C3DMaterial.h"
#include "C3DModel.h"
#include "C3DSkinlessModel.h"
#include "Geo.h"
#include "C3DDepthStencilTarget.h"
#include "C3DRenderTarget.h"

#include "C3DRenderSystem.h"
#include "C3DMesh.h"

#include "C3DMaterialManager.h"

namespace cocos3d
{
C3DPostEffect::C3DPostEffect(C3DPostProcess* postProcess, const std::string& szName)
	: _model( NULL ),
	_x(1),
	_y(1)
{
    _name = szName;

    _postProcess = postProcess;
    //_postProcess->retain();
    _material = NULL;
}

bool C3DPostEffect::init(const std::string& szMaterial)
{
	_szMaterial = szMaterial;

    SAFE_RELEASE(_material);

	_material = static_cast<C3DMaterial*>(C3DMaterialManager::getInstance()->getResource(szMaterial));
    if (_material == NULL)
        return false;

    _material->retain();
    C3DTechnique* technique = _material->getTechnique(0u);

	technique->getPass(0u)->getParameter("u_texture")->setValue(_postProcess->getFramebufferSampler());

	C3DMesh* mesh = Geo::createQuadFullscreen( 1, 1 );
	
	_model = C3DSkinlessModel::create();
	_model->retain();
	
	_model->setMesh( mesh );
	_model->setNode(_postProcess);

    return true;
}

// bool C3DPostEffect::reset()
// {
// 	SAFE_RELEASE(_model);
// 	SAFE_RELEASE(_material);
// 
// 	_material = static_cast<C3DMaterial*>(C3DMaterialManager::getInstance()->getResource(_szMaterial));
// 	if (_material == NULL)
// 		return false;
// 
// 	_material->retain();
// 	C3DTechnique* technique = _material->getTechnique(0u);
// 
// 	technique->getPass(0u)->getParameter("u_texture")->setValue(_postProcess->getFramebufferSampler());
// 
// 	C3DMesh* mesh = Geo::createQuadFullscreen(_x, _y);
// 	_model = C3DSkinlessModel::create();
// 	_model->setMesh(mesh);
// 	SAFE_RELEASE(mesh);
// 
// 	_model->setNode(_postProcess);
// 
// 	return true;
// }

void C3DPostEffect::setGridSize( unsigned int x, unsigned int y )
{
	_x = std::max<size_t>(x, 1);
	_y = std::max<size_t>(y, 1);

	reload();
}

void C3DPostEffect::reload()
{
	Geo::createQuadFullscreen( _x, _y, _model->getMesh());
}

C3DPostEffect::~C3DPostEffect()
{
	SAFE_RELEASE(_model);
    SAFE_RELEASE(_material);
}

void C3DPostEffect::draw()
{
    if (_material == NULL)
        return;

    if( _model->getMaterial() != _material)
    {
        _model->setMaterial(_material);
    }

    setShaderParameter();

    _model->draw();
}

}