#include "HLSLLBuffer.h"
#include "D3D9RigidMesh.h"
#include "D3D9Material.h"

CHLSLLBuffer::CHLSLLBuffer()
{
}
CHLSLLBuffer::~CHLSLLBuffer()
{
}
HRESULT CHLSLLBuffer::initialize(LPDIRECT3DDEVICE9 pd3dDevice, char *pszFileName)
{
	CD3D9Shader::initialize(pd3dDevice, pszFileName) ;

	setHandle(&m_hmatInvVP, "matInvVP") ;
	setHandle(&m_hvLightColor, "vLightColor") ;
	setHandle(&m_hvLightDir, "vLightDir") ;
	setHandle(&m_hvLightPos, "vLightPos") ;
	setHandle(&m_hvCameraPos, "vCameraPos") ;
	setHandle(&m_hfLightRange, "fLightRange") ;

	return S_OK ;
}
void CHLSLLBuffer::setParametersToShader(CSecretMesh *pcMesh)
{
	m_pEffect->SetMatrix(m_hmatInvVP, &m_matInvVP) ;
	m_pEffect->SetValue(m_hvLightColor, m_vLightColor, sizeof(D3DXVECTOR3)) ;
	m_pEffect->SetValue(m_hvLightDir, m_vLightDir, sizeof(D3DXVECTOR3)) ;
	m_pEffect->SetValue(m_hvLightPos, m_vLightPos, sizeof(D3DXVECTOR3)) ;
	m_pEffect->SetValue(m_hvCameraPos, m_vCameraPos, sizeof(D3DXVECTOR3)) ;
	m_pEffect->SetFloat(m_hfLightRange, m_fLightRange) ;
}
void CHLSLLBuffer::initializeParameters()
{
}

#include "SecretInput.h"
#include <windows.h>

void CHLSLLBuffer::tuneParameters(float t)
{
	float ratio = 1.0f*t ;
	bool bShift=false ;
	if(ScanKeyCode(VK_SHIFT))
		bShift=true ;

	float *pfParameter=NULL ;
	if(ScanKeyCode('U'))
	{
		m_vLightPos.y += ratio ;
	}
	if(ScanKeyCode('J'))
	{
		m_vLightPos.y -= ratio ;
	}
	if(ScanKeyCode('H'))
	{
		m_vLightPos.x -= ratio ;
	}
	if(ScanKeyCode('K'))
	{
		m_vLightPos.x += ratio ;
	}
}
