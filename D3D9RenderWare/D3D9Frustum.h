/*******************************
 *   @file		D3D9Frustum.h
 *   @author	kim do hyun
 *   @version	1.0
 *   @date		2012.05.14
 *   @note		SecretEngine for Direct3D9
 ********************************/

#pragma once

#include "D3D9Def.h"
#include "SecretFrustum.h"
#include "D3D9CoordSystem.h"

class CSecretShader ;

class CD3D9Frustum : public CSecretFrustum
{
public :

protected :
	LPDIRECT3DDEVICE9 m_pd3dDevice ;
	bool m_bDrawPlane ;

	CD3D9CoordSystem m_cLocalSystem ;

public :
	CD3D9Frustum() ;
	virtual ~CD3D9Frustum() ;

	void initialize(LPDIRECT3DDEVICE9 pd3dDevice) ;
	void buildFrustum(D3DXMATRIX *pmatV, D3DXMATRIX *pmatP) ;
	virtual void render(DWORD dwColor) ;

	void render(Matrix4 *pmat, CSecretShader *pcShader, int nPass, DWORD dwColor) ;
} ;