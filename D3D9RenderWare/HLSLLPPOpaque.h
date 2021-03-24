#pragma once

#include "D3D9Shader.h"

class CHLSLLPPOpaque : public CD3D9Shader
{
public :
	LPDIRECT3DTEXTURE9 m_pTexLight ;
	LPDIRECT3DTEXTURE9 m_pTexSSAO;
	D3DXVECTOR2 m_vOffset ;

protected :
	D3DXHANDLE m_hmatWVP ;
	D3DXHANDLE m_hvOffset ;

public :
	CHLSLLPPOpaque() ;
	virtual ~CHLSLLPPOpaque() ;

	virtual HRESULT initialize(LPDIRECT3DDEVICE9 pd3dDevice, char *pszFileName) ;

	virtual void setParametersToShader(CSecretMesh *pcMesh) ;
	virtual void initializeParameters() ;
} ;