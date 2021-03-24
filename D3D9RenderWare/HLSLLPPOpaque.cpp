#include "HLSLLPPOpaque.h"
#include "D3D9RigidMesh.h"
#include "D3D9RigidBumpMesh.h"
#include "D3D9Material.h"

CHLSLLPPOpaque::CHLSLLPPOpaque()
{
	m_pTexLight = NULL ;
	m_pTexSSAO = NULL;
	m_vOffset.x = 0.000767692f ;
	m_vOffset.y = 0.00208922f ;
}
CHLSLLPPOpaque::~CHLSLLPPOpaque()
{
}
HRESULT CHLSLLPPOpaque::initialize(LPDIRECT3DDEVICE9 pd3dDevice, char *pszFileName)
{
	CD3D9Shader::initialize(pd3dDevice, pszFileName) ;

	setHandle(&m_hmatWVP, "matWVP") ;
	setHandle(&m_hvOffset, "vOffset") ;

	return S_OK ;
}
void CHLSLLPPOpaque::setParametersToShader(CSecretMesh *pcMesh)
{
	D3DXMATRIX matWVP = m_pcIngredient->m_matWorld * m_pcIngredient->m_matView * m_pcIngredient->m_matProj ;

	m_pEffect->SetMatrix(m_hmatWVP, &matWVP) ;

	int nKind = pcMesh->getKind() ;

	if(nKind == SECRETNODE_KIND_RIGID)
	{
		CD3D9RigidMesh *pcRigid = (CD3D9RigidMesh *)pcMesh ;
		int nMaterialAttr = pcRigid->getD3D9Material()->getAttr() ;
		if(nMaterialAttr & CD3D9Material::MATERIAL_ATTR_DIFFUSE)
			m_pEffect->SetTexture("DiffuseMap", pcRigid->getD3D9Material()->m_pTexDiffuse) ;
	}
	else if(nKind == SECRETNODE_KIND_RIGIDBUMP)
	{
		CD3D9RigidBumpMesh *pcRigidBump = (CD3D9RigidBumpMesh *)pcMesh ;
		int nMaterialAttr = pcRigidBump->getD3D9Material()->getAttr() ;
		if(nMaterialAttr & CD3D9Material::MATERIAL_ATTR_DIFFUSE)
			m_pEffect->SetTexture("DiffuseMap", pcRigidBump->getD3D9Material()->m_pTexDiffuse) ;
	}

	if(m_pTexLight)
		m_pEffect->SetTexture("LightMap", m_pTexLight) ;
	if (m_pTexSSAO)
		m_pEffect->SetTexture("SSAOMap", m_pTexSSAO);

	m_pEffect->SetValue(m_hvOffset, &m_vOffset, sizeof(D3DXVECTOR2)) ;
}
void CHLSLLPPOpaque::initializeParameters()
{
}
