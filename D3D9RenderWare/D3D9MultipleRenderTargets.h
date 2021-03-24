/*******************************
 *   @file		D3D9MultiRenderTargets.h
 *   @author	kim do hyun
 *   @version	1.0
 *   @date		2011.12.14
 *   @note		SecretEngine for Direct3D9
 ********************************/

#pragma once

#include "D3D9Def.h"
#include <vector>

class CD3D9MultipleRenderTargets
{
public :
	struct SRenderTarget
	{
		LPDIRECT3DTEXTURE9 m_pTex ;
		LPDIRECT3DSURFACE9 m_pSurf ;

		SRenderTarget() ;
		~SRenderTarget() ;
		void release() ;
	} ;

	LPDIRECT3DDEVICE9 m_pd3dDevice ;
	std::vector<SRenderTarget *> m_cRenderTargets ;

	int m_nWidth, m_nHeight ;
	LPDIRECT3DSURFACE9 m_pSurfZBuffer ;
	D3DVIEWPORT9 m_Viewport ;

	LPDIRECT3DSURFACE9 m_pOldSurfBack, m_pOldSurfZBuffer ;
	D3DVIEWPORT9 m_OldViewport ;

public :
	CD3D9MultipleRenderTargets() ;
	~CD3D9MultipleRenderTargets() ;
	void release() ;

	HRESULT initialize(LPDIRECT3DDEVICE9 pd3dDevice, LPDIRECT3D9 pd3d9, int nWidth, int nHeight, D3DFORMAT d3dfmt_depth=D3DFMT_D24S8) ;
	HRESULT addRenderTarget(D3DFORMAT d3dfmt_rgb) ;

	SRenderTarget *getRenderTarget(int i) ;

	LPDIRECT3DTEXTURE9 getTexture(int i) ;


	void storeBackBuffer() ;
	void setRenderTarget() ;
	void restoreBackbuffer() ;
	void renderQuad(int nNumRT, int x, int y, int nWidth, int nHeight) ;

} ;