/*******************************
 *   @file		D3D9LPPOpaque.h
 *   @author	kim do hyun
 *   @version	1.0
 *   @date		2011.12.14
 *   @note		SecretEngine for Direct3D9
 ********************************/

#pragma once

#include "D3D9Def.h"
#include "SecretRigidMesh.h"

class CD3D9Material ;
class CD3D9TextureContainer ;
class CD3D9Shader ;

class CD3D9LPPOpaqueMesh : public CSecretRigidMesh
{
public :
	struct SVertex
	{
		D3DXVECTOR3 pos ;
		D3DXVECTOR3 normal ;
	} ;
	struct SIndex
	{
		unsigned short index[3] ;
	} ;

protected :
	LPDIRECT3DDEVICE9 m_pd3dDevice ;
	CD3D9Material *m_pcD3D9Material ;

	LPDIRECT3DVERTEXBUFFER9 m_pVB ;
	LPDIRECT3DINDEXBUFFER9 m_pIB ;

	LPDIRECT3DVERTEXDECLARATION9 m_pDecl ;

public :
	CD3D9Shader *m_pcShader ;

public :
	CD3D9LPPOpaqueMesh() ;
	virtual ~CD3D9LPPOpaqueMesh() ;

	void release() ;
	HRESULT initialize(LPDIRECT3DDEVICE9 pd3dDevice, SMesh *psMesh) ;
	virtual void render() ;
	
	void setMaterial(CD3D9Material *pcMaterial, CD3D9TextureContainer *pcTexContainer) ;
	CD3D9Material *getD3D9Material() ;

} ;
