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

#include "C3DSkinModel.h"
#include "C3DMesh.h"
#include "C3DSubMesh.h"
#include "C3DMeshSkin.h"
#include "C3DTechnique.h"
#include "C3DPass.h"
#include "C3DMaterial.h"
#include "BonePart.h"
#include "C3DStat.h"
#include "C3DRenderSystem.h"

namespace cocos3d
{
C3DSkinModel::C3DSkinModel() :_skin(NULL)
{
}

C3DSkinModel::~C3DSkinModel()
{
    SAFE_DELETE(_skin);
}

C3DModel* C3DSkinModel::create()
{
    return new C3DSkinModel();
}

C3DMeshSkin* C3DSkinModel::getSkin()
{
    return _skin;
}

void C3DSkinModel::setSkin(C3DMeshSkin* skin)
{
    if (_skin != skin)
    {
        // Free the old skin
        SAFE_DELETE(_skin);

        // Assign the new skin
        _skin = skin;
        if (_skin)
            _skin->_model = this;
    }
}

void C3DSkinModel::draw()
{
    C3DMaterial::TechniqueUsage techUsage =
        getNode()->get3DScene()->isInShadowPass() ? C3DMaterial::TECH_USAGE_SHADOWMAP : C3DMaterial::TECH_USAGE_SCREEN;

    unsigned int partCount = _mesh->getSubMeshCount();
    if (partCount == 0)
    {
        // No mesh parts (index buffers).
        if (_material)
        {
            C3DTechnique* technique = _material->getTechnique(techUsage);

            if (!technique)
                return;

			//**
            unsigned int passCount = technique->getPassCount();
            STAT_INC_DRAW_CALL(passCount);
            for (unsigned int i = 0; i < passCount; ++i)
            {
                STAT_INC_TRIANGLE_DRAW(_mesh->getTriangleCount());

                C3DPass* pass = technique->getPass(i);
				applyInternalParam(pass);
                //applyLightParam(pass);
                //applyShadowMap(pass);

                pass->bind();
                GL_ASSERT( glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0) );

				if (_wireframe && (_mesh->getPrimitiveType() == PrimitiveType_TRIANGLES || _mesh->getPrimitiveType() == PrimitiveType_TRIANGLE_STRIP))
				{
					unsigned int vertexCount = _mesh->getVertexCount();
					for (unsigned int j = 0; j < vertexCount; j += 3)
					{
						GL_ASSERT( glDrawArrays(GL_LINE_LOOP, j, 3) );
					}
				}
				else
				{
					GL_ASSERT( glDrawArrays(_mesh->getPrimitiveType(), 0, _mesh->getVertexCount()) );
				}
                pass->unbind();
            }
			//*/
        }
    }
    else
    {
        for (unsigned int i = 0; i < partCount; ++i)
        {
            C3DSubMesh* C3DSubMesh = _mesh->getSubMesh(i);

            // Get the material for this mesh part.
            C3DMaterial* material = getMaterial(i);
            if (material)
            {
                C3DTechnique* technique = _material->getTechnique(techUsage);

                if (!technique)
                    continue;

				//**
                unsigned int passCount = technique->getPassCount();

                for (unsigned int j = 0; j < passCount; ++j)
                {
                    STAT_INC_TRIANGLE_DRAW(C3DSubMesh->getTriangleCount());

                    C3DPass* pass = technique->getPass(j);
                    //applyLightParam(pass);
                    //applyShadowMap(pass);
					applyInternalParam(pass);

                    pass->bind();
                    GL_ASSERT( glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, C3DSubMesh->_indexBuffer) );

					C3DMeshSkin * skin = this->getSkin();
					if(skin != NULL)
					{
						BonePart** parts = skin->_parts;

                        STAT_INC_DRAW_CALL(skin->_partCount);

						for (unsigned int iBonePart=0; iBonePart < skin->_partCount; iBonePart++)
						{
							BonePart* bonePart = parts[iBonePart];
							skin->setBonePartIndex(iBonePart);
							pass->setParamMethonAutoUniform();

							if (_wireframe && (_mesh->getPrimitiveType() == PrimitiveType_TRIANGLES || _mesh->getPrimitiveType() == PrimitiveType_TRIANGLE_STRIP))
							{
								unsigned int indexCount = bonePart->_numVertexIndex;
								unsigned int indexSize = 0;
								switch (C3DSubMesh->getIndexFormat())
								{
								case IndexFormat_INDEX8:
									indexSize = 1;
									break;
								case IndexFormat_INDEX16:
									indexSize = 2;
									break;
								case IndexFormat_INDEX32:
									indexSize = 4;
									break;
								}

								for (unsigned int k = 0; k < indexCount; k += 3)
								{
									GL_ASSERT( glDrawElements(GL_LINE_LOOP, 3, C3DSubMesh->getIndexFormat(), ((const GLvoid*)(k*indexSize))) );
								}
							}
							else
							{
								GL_ASSERT( glDrawElements(C3DSubMesh->getPrimitiveType(),bonePart->_numVertexIndex , C3DSubMesh->getIndexFormat(), &((const GLushort*)0)[bonePart->_offsetVertexIndex]));
							}
						}
					}
                    GL_ASSERT( glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0) );
                    pass->unbind();
                }
				//*/
            }
        }
    }
}

C3DSkinModel* C3DSkinModel::clone(C3DNode::CloneContext& context) const
{
    C3DSkinModel* model = new C3DSkinModel();

    model->copyFrom(this);

    C3DMeshSkin *skin = new C3DMeshSkin();

    skin->copyFrom(_skin, context);

    model->setSkin(skin);

	model->autorelease();

    return model;
}

std::string C3DSkinModel::getDefaultMaterialName()
{
	return "common\\default_skin.material";
}

}