#pragma once

#include "D3D9Shader.h"

class CHLSLRigid : public CD3D9Shader
{
public :
	D3DXMATRIX m_matLVP ;
	D3DXVECTOR4 m_vNTs[3] ;
	LPDIRECT3DTEXTURE9 m_pTexTSM ;

protected :
	D3DXHANDLE m_hmatWVP ;
	D3DXHANDLE m_hmatWLVP ;
	D3DXHANDLE m_hvNTs ;

public :
	CHLSLRigid() ;
	virtual ~CHLSLRigid() ;

	virtual HRESULT initialize(LPDIRECT3DDEVICE9 pd3dDevice, char *pszFileName) ;

	virtual void setParametersToShader(CSecretMesh *pcMesh) ;
	virtual void initializeParameters() ;

} ;