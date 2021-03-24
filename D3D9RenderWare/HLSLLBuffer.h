#pragma once

#include "D3D9Shader.h"

class CHLSLLBuffer : public CD3D9Shader
{
public :
	D3DXMATRIX m_matInvVP ;
	D3DXVECTOR3 m_vLightColor ;
	D3DXVECTOR3 m_vLightDir ;
	D3DXVECTOR3 m_vLightPos ;
	D3DXVECTOR3 m_vCameraPos ;
	float m_fLightRange ;

protected :
	D3DXHANDLE m_hmatInvVP ;
	D3DXHANDLE m_hvLightColor ;
	D3DXHANDLE m_hvLightDir ;
	D3DXHANDLE m_hvLightPos ;
	D3DXHANDLE m_hvCameraPos ;
	D3DXHANDLE m_hfLightRange ;

public :
	CHLSLLBuffer() ;
	virtual ~CHLSLLBuffer() ;

	virtual HRESULT initialize(LPDIRECT3DDEVICE9 pd3dDevice, char *pszFileName) ;

	virtual void setParametersToShader(CSecretMesh *pcMesh) ;
	virtual void tuneParameters(float t) ;
	virtual void initializeParameters() ;
} ;