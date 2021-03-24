#include "HLSLTSM.h"
#include "D3D9RigidMesh.h"
#include "D3D9RigidBumpMesh.h"
#include "D3D9Material.h"

CHLSLTSM::CHLSLTSM()
{
}
CHLSLTSM::~CHLSLTSM()
{
}
HRESULT CHLSLTSM::initialize(LPDIRECT3DDEVICE9 pd3dDevice, char *pszFileName)
{
	CD3D9Shader::initialize(pd3dDevice, pszFileName) ;
	setHandle(&m_hmatWLVP, "matWLVP") ;
	setHandle(&m_hvNTs, "vNTs") ;

	return S_OK ;
}
void CHLSLTSM::setParametersToShader(CSecretMesh *pcMesh)
{
	D3DXMATRIX matWLVP = m_pcIngredient->m_matWorld * m_matLVP ;
	m_pEffect->SetMatrix(m_hmatWLVP, &matWLVP) ;

	m_pEffect->SetVectorArray(m_hvNTs, m_vNTs, 3) ;

}
void CHLSLTSM::initializeParameters()
{
}
