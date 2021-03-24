#pragma once

#include "D3D9Shader.h"

class CHLSLGBuffer : public CD3D9Shader
{
public :

protected :
	D3DXHANDLE m_hmatWVP ;
	D3DXHANDLE m_hmatW ;
	D3DXHANDLE m_hfShine ;
	D3DXHANDLE m_hfShineStrength ;

public :
	CHLSLGBuffer() ;
	virtual ~CHLSLGBuffer() ;

	virtual HRESULT initialize(LPDIRECT3DDEVICE9 pd3dDevice, char *pszFileName) ;

	virtual void setParametersToShader(CSecretMesh *pcMesh) ;
	virtual void initializeParameters() ;
} ;