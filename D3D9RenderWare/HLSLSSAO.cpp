#include "HLSLSSAO.h"
#include "def.h"

CHLSLSSAO::CHLSLSSAO() :
MAX_SAMPLES(12)
{
	m_pvSampleOffsets = NULL ;

	m_fSample_Rad=2.0f ;
	m_fScale=1.0f ;
	m_fBias=0.001f ;
	m_fIntensity=1.0f ;

	m_fNearToFar = 1024.0f-0.1f ;
}
CHLSLSSAO::~CHLSLSSAO()
{
	release() ;
}
void CHLSLSSAO::release()
{
	SAFE_DELETEARRAY(m_pvSampleOffsets) ;
}
HRESULT CHLSLSSAO::initialize(LPDIRECT3DDEVICE9 pd3dDevice, char *pszFileName)
{
	CD3D9Shader::initialize(pd3dDevice, pszFileName) ;

	setHandle(&m_havSampleOffsets, "avSampleOffsets") ;
	setHandle(&m_hmatInvProj, "matInvProj") ;
	setHandle(&m_hmatInvVP, "matInvVP") ;
	setHandle(&m_hmatView, "matView") ;
	setHandle(&m_hvScreenSize, "vScreenSize") ;
	setHandle(&m_hvRandomSize, "vRandomSize") ;

	setHandle(&m_hfSample_Rad, "sample_rad") ;
	setHandle(&m_hfScale, "scale") ;
	setHandle(&m_hfBias, "bias") ;
	setHandle(&m_hfIntensity, "intensity") ;
	setHandle(&m_hfNearToFar, "fNearToFar") ;

	m_pvSampleOffsets = new D3DXVECTOR2[MAX_SAMPLES] ;

	m_pvSampleOffsets[0] = D3DXVECTOR2(-1.0f, 0.0f) ;
	m_pvSampleOffsets[1] = D3DXVECTOR2(-2.0f, 0.0f) ;

	m_pvSampleOffsets[2] = D3DXVECTOR2(1.0f, 0.0f) ;
	m_pvSampleOffsets[3] = D3DXVECTOR2(2.0f, 0.0f) ;

	m_pvSampleOffsets[4] = D3DXVECTOR2(0.0f, 1.0f) ;
	m_pvSampleOffsets[5] = D3DXVECTOR2(0.0f, 2.0f) ;

	m_pvSampleOffsets[6] = D3DXVECTOR2(0.0f, -1.0f) ;
	m_pvSampleOffsets[7] = D3DXVECTOR2(0.0f, -2.0f) ;

	m_pvSampleOffsets[8] = D3DXVECTOR2(-1.0f, 1.0f) ;
	m_pvSampleOffsets[9] = D3DXVECTOR2(1.0f, 1.0f) ;

	m_pvSampleOffsets[10] = D3DXVECTOR2(-1.0f, -1.0f) ;
	m_pvSampleOffsets[11] = D3DXVECTOR2(1.0f, -1.0f) ;

	D3DXMatrixIdentity(&m_matInvProj) ;

	return S_OK ;
}
void CHLSLSSAO::setParametersToShader(CSecretMesh *pcMesh)
{
	m_pEffect->SetValue(m_havSampleOffsets, m_pvSampleOffsets, sizeof(D3DXVECTOR2)*MAX_SAMPLES) ;
	m_pEffect->SetMatrix(m_hmatInvProj, &m_matInvProj) ;
	m_pEffect->SetMatrix(m_hmatInvVP, &m_matInvVP) ;
	m_pEffect->SetMatrix(m_hmatView, &m_matView) ;

	m_pEffect->SetValue(m_hvScreenSize, &m_vScreenSize, sizeof(D3DXVECTOR2)) ;
	m_pEffect->SetValue(m_hvRandomSize, &m_vRandomSize, sizeof(D3DXVECTOR2)) ;
	m_pEffect->SetFloat(m_hfNearToFar, m_fNearToFar) ;

	m_pEffect->SetFloat(m_hfSample_Rad, m_fSample_Rad) ;
	m_pEffect->SetFloat(m_hfScale, m_fScale) ;
	m_pEffect->SetFloat(m_hfBias, m_fBias) ;
	m_pEffect->SetFloat(m_hfIntensity, m_fIntensity) ;
}
void CHLSLSSAO::initializeParameters()
{
}

#include "SecretInput.h"
#include <windows.h>

void CHLSLSSAO::tuneParameters(float t)
{
	float ratio = 1.0f*t ;
	bool bShift=false ;
	if(ScanKeyCode(VK_SHIFT))
		bShift=true ;

	float *pfParameter=NULL ;
	if(ScanKeyCode(0x30))//m_fSample_Rad
	{
		pfParameter = &m_fSample_Rad ;
		ratio = 0.1f*t ;
	}
	else if(ScanKeyCode(0x31))//m_fScale
	{
		pfParameter = &m_fScale ;
		ratio = 0.01f*t ;
	}
	else if(ScanKeyCode(0x32))//m_fBias
	{
		pfParameter = &m_fBias ;
		ratio = 0.01f*t ;
	}
	else if(ScanKeyCode(0x33))//m_fIntensity
		pfParameter = &m_fIntensity ;

	if(pfParameter)
	{
		if(bShift)
			(*pfParameter) -= ratio ;
		else
			(*pfParameter) += ratio ;
	}
}