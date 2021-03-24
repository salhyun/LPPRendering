#pragma once

#include "D3D9Shader.h"

class CHLSLSSAO : public CD3D9Shader
{
public :
	const int MAX_SAMPLES ;
	D3DXVECTOR2 *m_pvSampleOffsets ;
	D3DXMATRIX m_matInvProj ;
	D3DXMATRIX m_matInvVP ;
	D3DXMATRIX m_matView ;

	D3DXVECTOR2 m_vScreenSize ;
	D3DXVECTOR2 m_vRandomSize ;
	float m_fNearToFar ;

	float m_fSample_Rad ;
	float m_fScale ;
	float m_fBias ;
	float m_fIntensity ;

protected :
	D3DXHANDLE m_havSampleOffsets ;
	D3DXHANDLE m_hmatInvProj ;
	D3DXHANDLE m_hmatInvVP ;
	D3DXHANDLE m_hmatView ;
	D3DXHANDLE m_hvScreenSize ;
	D3DXHANDLE m_hvRandomSize ;
	D3DXHANDLE m_hfNearToFar ;

	D3DXHANDLE m_hfSample_Rad ;
	D3DXHANDLE m_hfScale ;
	D3DXHANDLE m_hfBias ;
	D3DXHANDLE m_hfIntensity ;

public :
	CHLSLSSAO() ;
	virtual ~CHLSLSSAO() ;

	void release() ;

	virtual HRESULT initialize(LPDIRECT3DDEVICE9 pd3dDevice, char *pszFileName) ;

	virtual void setParametersToShader(CSecretMesh *pcMesh) ;
	virtual void tuneParameters(float t) ;
	virtual void initializeParameters() ;
} ;