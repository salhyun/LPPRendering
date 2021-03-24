#pragma once

#include "D3D9Def.h"
#include "SecretRenderingManager.h"

class CD3D9RenderingManager : public CSecretRenderingManager
{
public :

protected :
	LPDIRECT3DDEVICE9 m_pd3dDevice ;

public :
	CD3D9RenderingManager() ;
	virtual ~CD3D9RenderingManager() ;

	void initialize(LPDIRECT3DDEVICE9 pd3dDevice, int nMaxNumNode, CSecretCamera *pcCamera, CSecretFrustum *pcFrustum, float fCameraBoundingSphereRadius) ;

	virtual void render(CSecretShader *pcShader=NULL, int nPass=0) ;
	virtual void renderOpaque(CSecretShader *pcShader=NULL, int nPass=0) ;
	virtual void renderOpaqueRigid(CSecretShader *pcShader=NULL, int nPass=0) ;
	virtual void renderOpaqueSkinned(CSecretShader *pcShader=NULL, int nPass=0) ;
	virtual void renderTransparent(CSecretShader *pcShader=NULL, int nPass=0) ;
} ;