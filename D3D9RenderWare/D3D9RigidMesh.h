/*******************************
 *   @file		D3D9RigidMesh.h
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
class CD3D9LPPOpaqueMesh ;

class CD3D9RigidMesh : public CSecretRigidMesh
{
public :
	struct SVertex
	{
		D3DXVECTOR3 pos ;
		D3DXVECTOR3 normal ;
		D3DXVECTOR2 tex ;
	} ;
	struct SIndex
	{
		unsigned short index[3] ;
	} ;

protected :
	LPDIRECT3DDEVICE9 m_pd3dDevice ;
	
	LPDIRECT3DVERTEXBUFFER9 m_pVB ;
	LPDIRECT3DINDEXBUFFER9 m_pIB ;

	LPDIRECT3DVERTEXDECLARATION9 m_pDecl ;

public :
	//CD3D9Shader *m_pcShader ;
	//CD3D9LPPOpaqueMesh *m_pcOpaqueMesh ;

public :
	CD3D9RigidMesh() ;
	virtual ~CD3D9RigidMesh() ;

	void release() ;
	HRESULT initialize(LPDIRECT3DDEVICE9 pd3dDevice, SMesh *psMesh) ;
	virtual void render() ;
	
	void setMaterial(CD3D9Material *pcMaterial, CD3D9TextureContainer *pcTexContainer) ;
	CD3D9Material *getD3D9Material() ;

} ;
