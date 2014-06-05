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

#include "C3DSkinlessModel.h"
#include "C3DMesh.h"
#include "C3DSubMesh.h"
#include "C3DTechnique.h"
#include "C3DPass.h"
#include "C3DMaterial.h"
#include "C3DStat.h"

namespace cocos3d
{
C3DSkinlessModel::C3DSkinlessModel()
{
}

C3DSkinlessModel::~C3DSkinlessModel()
{
}

C3DModel* C3DSkinlessModel::create()
{
    return new C3DSkinlessModel();
}

C3DModel* C3DSkinlessModel::clone(C3DNode::CloneContext& context) const
{
	C3DSkinlessModel* other = new C3DSkinlessModel();

	other->copyFrom(this);
	other->autorelease();
	return other;
}

std::string C3DSkinlessModel::getDefaultMaterialName()
{
	return "common\\default.material";
}

void C3DSkinlessModel::draw(void)
{
	unsigned int partCount = _mesh->getSubMeshCount();

	C3DMaterial::TechniqueUsage techUsage =
		getNode()->get3DScene()->isInShadowPass() ? C3DMaterial::TECH_USAGE_SHADOWMAP : C3DMaterial::TECH_USAGE_SCREEN;

	if ( partCount == 0 )
	{
		// No mesh parts (index buffers).
		if (_material)
		{
			C3DTechnique* technique = _material->getTechnique(techUsage);

			unsigned int passCount = technique->getPassCount();
			
			STAT_INC_DRAW_CALL(passCount);

			for (unsigned int i = 0; i < passCount; ++i)
			{
				STAT_INC_TRIANGLE_DRAW(_mesh->getTriangleCount());

				C3DPass* pass = technique->getPass(i);
				//applyLightParam(pass);
				//applyShadowMap(pass);
				applyInternalParam(pass);
				pass->bind();
				GL_ASSERT( glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0) );
				if (_wireframe && (_mesh->getPrimitiveType() == GL_TRIANGLES || _mesh->getPrimitiveType() == GL_TRIANGLE_STRIP))
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
		}
	}
	else
	{
		for ( size_t i = 0; i < partCount; ++i )
		{
			C3DMaterial::TechniqueUsage techUsage =
				getNode()->get3DScene()->isInShadowPass() ? C3DMaterial::TECH_USAGE_SHADOWMAP : C3DMaterial::TECH_USAGE_SCREEN;

			C3DMaterial* material = getMaterial(i);
			C3DSubMesh* subMesh = _mesh->getSubMesh(i);

			if (material)
			{
				C3DTechnique* technique = material->getTechnique(techUsage);
				unsigned int passCount = technique->getPassCount();

				STAT_INC_DRAW_CALL(passCount);

				for (unsigned int j = 0; j < passCount; ++j)
				{
					STAT_INC_TRIANGLE_DRAW(subMesh->getTriangleCount());

					C3DPass* pass = technique->getPass(j);
					//applyLightParam(pass);
					//applyShadowMap(pass);
					applyInternalParam(pass);
					pass->bind();
					GL_ASSERT( glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, subMesh->getIndexBuffer()) );

					// check whether show the wireframe
					if (_wireframe && (_mesh->getPrimitiveType() == PrimitiveType_TRIANGLES || _mesh->getPrimitiveType() == PrimitiveType_TRIANGLE_STRIP))
					{
						unsigned int indexSize = 0;
						switch (subMesh->getIndexFormat())
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

						int indexCount = subMesh->getIndexCount();
						for (int k = 0; k < indexCount; k += 3)
						{
							GL_ASSERT( glDrawElements(GL_LINE_LOOP, 3, subMesh->getIndexFormat(), ((const GLvoid*)(k*indexSize))));
						}
					}
					else
					{
						GL_ASSERT( glDrawElements(subMesh->getPrimitiveType(), subMesh->getIndexCount(), subMesh->getIndexFormat(), 0) );
					}
					pass->unbind();
				}
			}
		}
	}
}

void C3DSkinlessModel::channelDrawPart( int partIndex )
{
	
}

}