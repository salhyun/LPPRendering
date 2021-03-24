#pragma once

#include "D3D9Shader.h"

class CHLSLLPPFinal : public CD3D9Shader
{
public :
	LPDIRECT3DTEXTURE9 m_pTexLight ;
	LPDIRECT3DTEXTURE9 m_pTexDiffuse ;

protected :
	D3DXHANDLE m_hmatWVP ;

public :
	CHLSLLPPFinal() ;
	virtual ~CHLSLLPPFinal() ;

	virtual HRESULT initialize(LPDIRECT3DDEVICE9 pd3dDevice, char *pszFileName) ;

	virtual void setParametersToShader(CSecretMesh *pcMesh) ;
	virtual void initializeParameters() ;
} ;