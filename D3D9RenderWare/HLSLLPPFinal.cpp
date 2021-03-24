#include "HLSLLPPFinal.h"
#include "D3D9RigidMesh.h"
#include "D3D9Material.h"

CHLSLLPPFinal::CHLSLLPPFinal()
{
	m_pTexLight = NULL ;
}
CHLSLLPPFinal::~CHLSLLPPFinal()
{
}
HRESULT CHLSLLPPFinal::initialize(LPDIRECT3DDEVICE9 pd3dDevice, char *pszFileName)
{
	CD3D9Shader::initialize(pd3dDevice, pszFileName) ;

	setHandle(&m_hmatWVP, "matWVP") ;

	return S_OK ;
}
void CHLSLLPPFinal::setParametersToShader(CSecretMesh *pcMesh)
{
	D3DXMATRIX matWVP = m_pcIngredient->m_matWorld * m_pcIngredient->m_matView * m_pcIngredient->m_matProj ;

	m_pEffect->SetMatrix(m_hmatWVP, &matWVP) ;

	m_pEffect->SetTexture("LightMap", m_pTexLight) ;
	m_pEffect->SetTexture("DiffuseMap", m_pTexDiffuse) ;
}
void CHLSLLPPFinal::initializeParameters()
{
}
