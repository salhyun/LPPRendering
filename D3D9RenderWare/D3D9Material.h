/*******************************
 *   @file		D3D9Material.h
 *   @author	kim do hyun
 *   @version	1.0
 *   @date		2011.12.14
 *   @note		SecretEngine for Direct3D9
 ********************************/

#pragma once

#include "D3D9Def.h"
#include "SecretMaterial.h"

class CD3D9Material : public CSecretMaterial
{
public :
	LPDIRECT3DTEXTURE9 m_pTexDiffuse ;
	LPDIRECT3DTEXTURE9 m_pTexNormal ;
	LPDIRECT3DTEXTURE9 m_pTexSpecular ;

protected :

public :
	CD3D9Material() ;
	virtual ~CD3D9Material() ;

	void release() ;

} ;
