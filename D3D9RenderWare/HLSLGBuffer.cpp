#include "HLSLGBuffer.h"
#include "D3D9RigidMesh.h"
#include "D3D9RigidBumpMesh.h"
#include "D3D9Material.h"

CHLSLGBuffer::CHLSLGBuffer()
{
}
CHLSLGBuffer::~CHLSLGBuffer()
{
}
HRESULT CHLSLGBuffer::initialize(LPDIRECT3DDEVICE9 pd3dDevice, char *pszFileName)
{
	CD3D9Shader::initialize(pd3dDevice, pszFileName) ;

	setHandle(&m_hmatWVP, "matWVP") ;
	setHandle(&m_hmatW, "matW") ;
	setHandle(&m_hfShine, "fShine") ;
	setHandle(&m_hfShineStrength, "fShineStrength") ;

	return S_OK ;
}
void CHLSLGBuffer::setParametersToShader(CSecretMesh *pcMesh)
{
	D3DXMATRIX matW = m_pcIngredient->m_matWorld ;
	D3DXMATRIX matWVP = m_pcIngredient->m_matWorld * m_pcIngredient->m_matView * m_pcIngredient->m_matProj ;

	m_pEffect->SetMatrix(m_hmatW, &matW) ;
	m_pEffect->SetMatrix(m_hmatWVP, &matWVP) ;

	if(pcMesh)
	{
		CSecretMaterial *material = pcMesh->getMaterial() ;
		m_pEffect->SetFloat(m_hfShine, material->m_fShine) ;
		m_pEffect->SetFloat(m_hfShineStrength, material->m_fShineStrength) ;

		int nKind = pcMesh->getKind() ;
		if(nKind == SECRETNODE_KIND_RIGIDBUMP)
		{
			CD3D9RigidBumpMesh *pcRigidBump = (CD3D9RigidBumpMesh *)pcMesh ;
			int nMaterialAttr = pcRigidBump->getD3D9Material()->getAttr() ;
			if(nMaterialAttr & CD3D9Material::MATERIAL_ATTR_NORMAL)
				m_pEffect->SetTexture("NormalMap", pcRigidBump->getD3D9Material()->m_pTexNormal) ;
		}
	}

}
void CHLSLGBuffer::initializeParameters()
{
}
