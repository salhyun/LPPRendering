#include "D3D9Model.h"
#include "D3D9ShaderManager.h"
#include "D3D9Shader.h"
#include "D3D9TextureContainer.h"
#include "D3D9Material.h"
#include "D3D9RigidMesh.h"
#include "D3D9RigidBumpMesh.h"
#include "SecretTNRModel.h"

CD3D9Model::CD3D9Model(LPDIRECT3DDEVICE9 pd3dDevice)
{
	m_pd3dDevice = pd3dDevice ;
	m_pcShaderManager = NULL ;
}
CD3D9Model::~CD3D9Model()
{
	release() ;
}
void CD3D9Model::release()
{
	SAFE_DELETEARRAY(m_pcD3D9Materials) ;
}
void CD3D9Model::_setParent(CSecretNode *node)
{
	if(node->isParent())
	{
		CSecretNode *parent = getNode(node->getParentNodeName()) ;
		if(parent)
		{
			node->setParentNode(parent) ;
		}
	}
}
void CD3D9Model::_setMaterial(CD3D9Material *dest, SMaterial *src)
{
	CSecretModel::_setMaterial(dest, src) ;
}
void CD3D9Model::initialize(CASEData *pcASEData, CD3D9TextureContainer *pcTexContainer, CD3D9ShaderManager *pcShaderManager)
{
	int i ;
	m_pcShaderManager = pcShaderManager ;

	CSecretModel::initialize(pcASEData) ;

	m_pcNodes = new std::vector<CSecretNode *> ;

	//get materials
	m_nNumMaterial = pcASEData->m_nNumMaterial ;
	if(m_nNumMaterial > 0)
	{
		m_pcD3D9Materials = new CD3D9Material[m_nNumMaterial] ;

		for(i=0; i<m_nNumMaterial; i++)
		{
			_setMaterial(&m_pcD3D9Materials[i], &pcASEData->m_psMaterial[i]) ;
		}
	}

	//setting meshes
	for(i=0 ; i<pcASEData->m_nNumMesh ; i++)
	{
		if(pcASEData->m_apMesh[i]->MeshKind == SMESHKIND_RIGID)
		{
			if(pcASEData->m_apMesh[i]->nAttr & SMESHATTR_USENORMALMAP)
			{
				CD3D9RigidBumpMesh *pcRigidBumpMesh = new CD3D9RigidBumpMesh() ;
				pcRigidBumpMesh->initialize(m_pd3dDevice, pcASEData->m_apMesh[i]) ;

				//set material
				pcRigidBumpMesh->setMaterial(&m_pcD3D9Materials[pcASEData->m_apMesh[i]->nMaterialIndex], pcTexContainer) ;
				int nRenderingAttr = RENDERING_ATTR_RIGID ;
				int nMaterialAttr = CSecretMaterial::MATERIAL_ATTR_DIFFUSE;// pcRigidMesh->getD3D9Material()->getAttr() ;

				//suited shader
				//pcRigidMesh->m_pcShader = pcShaderManager->getShader(0) ;
				pcRigidBumpMesh->m_pcShader = (CSecretShader *)pcShaderManager->getSuitedShader(nRenderingAttr, nMaterialAttr) ;

				//set parent
				_setParent((CSecretNode *)pcRigidBumpMesh) ;
				m_pcNodes->push_back((CSecretNode *)pcRigidBumpMesh) ;
			}
			else
			{
				CD3D9RigidMesh *pcRigidMesh = new CD3D9RigidMesh() ;
				pcRigidMesh->initialize(m_pd3dDevice, pcASEData->m_apMesh[i]) ;

				//set material
				pcRigidMesh->setMaterial(&m_pcD3D9Materials[pcASEData->m_apMesh[i]->nMaterialIndex], pcTexContainer) ;
				int nRenderingAttr = RENDERING_ATTR_RIGID ;
				int nMaterialAttr = CSecretMaterial::MATERIAL_ATTR_DIFFUSE;// pcRigidMesh->getD3D9Material()->getAttr() ;
				//int nMaterialAttr = pcRigidMesh->getD3D9Material()->getAttr() ;

				//suited shader
				//pcRigidMesh->m_pcShader = pcShaderManager->getShader(0) ;
				pcRigidMesh->m_pcShader = (CSecretShader *)pcShaderManager->getSuitedShader(nRenderingAttr, nMaterialAttr) ;

				//set parent
				_setParent((CSecretNode *)pcRigidMesh) ;
				m_pcNodes->push_back((CSecretNode *)pcRigidMesh) ;
			}
		}
		else if(pcASEData->m_apMesh[i]->MeshKind == SMESHKIND_TEXANI)
		{
		}
	}
}
void CD3D9Model::render(CSecretTNRModel *pcTNRModel, CSecretShader *pcShader, int nPass)
{
	std::vector<CSecretNode *>::iterator it ;
	for(it=m_pcNodes->begin(); it!=m_pcNodes->end(); it++)
	{
		render((CSecretMesh *)(*it), pcTNRModel, pcShader, nPass) ;
	}
}
void CD3D9Model::render(CSecretMesh *pcNode, CSecretTNRModel *pcTNRModel, CSecretShader *pcShader, int nPass)
{
	Matrix4 matWorld ;
	int nNodeKind = pcNode->getKind() ;
	CD3D9Shader *pcd3dShader = (CD3D9Shader *)pcShader ;

	CSecretMesh *pcMesh = (CSecretMesh *)pcNode ;

	if(pcd3dShader)
	{
		pcd3dShader->setTechnique() ;
		pcd3dShader->begin() ;
		pcd3dShader->beginPass(nPass) ;
	}
	else
	{
		pcd3dShader = (CD3D9Shader *)pcMesh->m_pcShader ;
		pcd3dShader->setTechnique() ;
		pcd3dShader->begin() ;
		pcd3dShader->beginPass(pcMesh->m_nPass) ;
	}

	if(pcMesh->isAttr(SECRETNODE_ATTR_INHERITFROMBONE) || pcMesh->isParent())
	{
		matWorld = (*pcMesh->GetmatTM()) * (*pcTNRModel->getTransform()) ;
	}
	else
	{
		matWorld = (*pcMesh->GetmatLocal()) * (*pcTNRModel->getTransform()) ;
	}

	CRenderingIngredient *pcIngredient = pcd3dShader->getIngredient() ;
	if(pcIngredient)
	{
		D3DXMATRIX matResult ;
		COPY_D3DXMATRIX(matResult, matWorld) ;
		pcIngredient->m_matWorld = matResult ;
	}
	pcd3dShader->setParametersToShader(pcMesh) ;
	pcd3dShader->m_pEffect->CommitChanges() ;
	pcMesh->render() ;
	
	pcd3dShader->endPass() ;
	pcd3dShader->end() ;
}
CSecretMaterial *CD3D9Model::getMaterial(int nNum)
{
	return (CSecretMaterial *)(&m_pcD3D9Materials[nNum]) ;
}
void CD3D9Model::render(CSecretNode *pcNode, Matrix4 *pmat, CSecretShader *pcShader, int nPass)
{
	Matrix4 matWorld ;
	int nNodeKind = pcNode->getKind() ;
	CD3D9Shader *pcd3dShader = (CD3D9Shader *)pcShader ;

	CSecretMesh *pcMesh = (CSecretMesh *)pcNode ;

	if(pcd3dShader)
	{
		pcd3dShader->setTechnique() ;
		pcd3dShader->begin() ;
		pcd3dShader->beginPass(nPass) ;
	}
	else
	{
		pcd3dShader = (CD3D9Shader *)pcMesh->m_pcShader ;
		pcd3dShader->setTechnique() ;
		pcd3dShader->begin() ;
		pcd3dShader->beginPass(pcMesh->m_nPass) ;
	}

	if(pcMesh->isAttr(SECRETNODE_ATTR_INHERITFROMBONE) || pcMesh->isParent())
	{
		matWorld = (*pcMesh->GetmatTM()) * (*pmat) ;
	}
	else
	{
		matWorld = (*pcMesh->GetmatLocal()) * (*pmat) ;
	}

	CRenderingIngredient *pcIngredient = pcd3dShader->getIngredient() ;
	if(pcIngredient)
	{
		D3DXMATRIX matResult ;
		COPY_D3DXMATRIX(matResult, matWorld) ;
		pcIngredient->m_matWorld = matResult ;
	}
	pcd3dShader->setParametersToShader(pcMesh) ;
	pcd3dShader->m_pEffect->CommitChanges() ;
	pcMesh->render() ;
	
	pcd3dShader->endPass() ;
	pcd3dShader->end() ;

	/*
	if(nNodeKind == SECRETNODE_KIND_RIGID)
	{
		Matrix4 matWorld ;
		CD3D9RigidMesh *pcRigidMesh = (CD3D9RigidMesh *)pcNode ;

		if(pcd3dShader)
		{
			pcd3dShader->setTechnique() ;
			pcd3dShader->begin() ;
			pcd3dShader->beginPass(nPass) ;
		}
		else
		{
			pcd3dShader = (CD3D9Shader *)pcRigidMesh->m_pcShader ;
			pcd3dShader->setTechnique() ;
			pcd3dShader->begin() ;
			pcd3dShader->beginPass(pcRigidMesh->m_nPass) ;
		}

		if(pcRigidMesh->isAttr(SECRETNODE_ATTR_INHERITFROMBONE))
		{
			matWorld = (*pcRigidMesh->GetmatTM()) * (*pmat) ;
		}
		else if(pcRigidMesh->isParent())
		{
			matWorld = (*pcRigidMesh->GetmatTM()) * (*pmat) ;
		}
		else
		{
			matWorld = (*pcRigidMesh->GetmatLocal()) * (*pmat) ;
		}

		CRenderingIngredient *pcIngredient = pcd3dShader->getIngredient() ;
		if(pcIngredient)
		{
			D3DXMATRIX matResult ;
			COPY_D3DXMATRIX(matResult, matWorld) ;
			pcIngredient->m_matWorld = matResult ;
		}
		pcd3dShader->setParametersToShader((CSecretMesh *)pcNode) ;
		pcd3dShader->m_pEffect->CommitChanges() ;
		pcRigidMesh->render() ;

		pcd3dShader->endPass() ;
		pcd3dShader->end() ;
	}
	*/
}
