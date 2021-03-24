/*******************************
 *   @file		D3D9CoordSystem.h
 *   @author	kim do hyun
 *   @version	1.0
 *   @date		2012.05.14
 *   @note		SecretEngine for Direct3D9
 ********************************/

#pragma once

#include "D3D9Def.h"
#include "SecretCoordSystem.h"

class CD3D9CoordSystem : public CSecretCoordSystem
{
public :

protected :
	LPDIRECT3DDEVICE9 m_pd3dDevice ;
	D3DXMATRIX m_d3dmatPivot ;

public :
	CD3D9CoordSystem() ;
	virtual ~CD3D9CoordSystem() ;

	void initialize(LPDIRECT3DDEVICE9 pd3dDevice) ;
	void setMatrix(D3DXMATRIX *pmat) ;
	virtual void render() ;
} ;
