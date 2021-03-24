#include "HLSLRigid.h"
#include "D3D9RigidMesh.h"
#include "D3D9Material.h"

CHLSLRigid::CHLSLRigid()
{
	D3DXMatrixIdentity(&m_matLVP) ;

	m_vNTs[0].x=0.0f;	m_vNTs[0].y=0.0f;	m_vNTs[0].z=0.0f;	m_vNTs[0].w=0.0f;
	m_vNTs[1].x=0.0f;	m_vNTs[1].y=0.0f;	m_vNTs[1].z=0.0f;	m_vNTs[1].w=0.0f;
	m_vNTs[2].x=0.0f;	m_vNTs[2].y=0.0f;	m_vNTs[2].z=0.0f;	m_vNTs[2].w=0.0f;

	m_pTexTSM = NULL ;
}
CHLSLRigid::~CHLSLRigid()
{
}
HRESULT CHLSLRigid::initialize(LPDIRECT3DDEVICE9 pd3dDevice, char *pszFileName)
{
	CD3D9Shader::initialize(pd3dDevice, pszFileName) ;

	setHandle(&m_hmatWVP, "matWVP") ;
	setHandle(&m_hmatWLVP, "matWLVP") ;
	setHandle(&m_hvNTs, "vNTs") ;

	return S_OK ;
}
void CHLSLRigid::setParametersToShader(CSecretMesh *pcMesh)
{
	D3DXMATRIX matWVP = m_pcIngredient->m_matWorld * m_pcIngredient->m_matView * m_pcIngredient->m_matProj ;

	m_pEffect->SetMatrix(m_hmatWVP, &matWVP) ;

	CD3D9RigidMesh *pcRigid = (CD3D9RigidMesh *)pcMesh ;
	int nMaterialAttr = pcRigid->getD3D9Material()->getAttr() ;
	if(nMaterialAttr & CD3D9Material::MATERIAL_ATTR_DIFFUSE)
		m_pEffect->SetTexture("DiffuseMap", pcRigid->getD3D9Material()->m_pTexDiffuse) ;

	D3DXMATRIX mat = m_pcIngredient->m_matWorld * m_matLVP ;
	m_pEffect->SetMatrix(m_hmatWLVP, &mat) ;

	m_pEffect->SetVectorArray(m_hvNTs, m_vNTs, 3) ;

	m_pEffect->SetTexture("ShadowMap", m_pTexTSM) ;

}
void CHLSLRigid::initializeParameters()
{
}
