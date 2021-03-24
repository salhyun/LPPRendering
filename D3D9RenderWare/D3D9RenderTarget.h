/*******************************
 *   @file		D3D9RenderTarget.h
 *   @author	kim do hyun
 *   @version	1.0
 *   @date		2011.12.14
 *   @note		SecretEngine for Direct3D9
 ********************************/

#pragma once

#include "D3D9Def.h"

class CD3D9RenderTarget
{
public :
	LPDIRECT3DDEVICE9 m_pd3dDevice ;
	LPDIRECT3DTEXTURE9 m_pTex ;
	LPDIRECT3DSURFACE9 m_pSurf ;
	LPDIRECT3DSURFACE9 m_pSurfZBuffer ;
	D3DVIEWPORT9 m_Viewport ;
	int m_nHeight, m_nWidth ;
	float m_fNear, m_fFar ;

	LPDIRECT3DSURFACE9 m_pOldSurfBack, m_pOldSurfZBuffer ;
	D3DVIEWPORT9 m_OldViewport ;

	char m_szName[256] ;

public :
	CD3D9RenderTarget() ;
	~CD3D9RenderTarget() ;

	void release() ;

	HRESULT initialize(LPDIRECT3DDEVICE9 pd3dDevice, int nWidth, int nHeight, D3DFORMAT d3dfmt_rgb, D3DFORMAT d3dfmt_depth, float fNear, float fFar) ;

	void storeBackBuffer() ;
	void setRenderTarget() ;
	void restoreBackbuffer() ;
	void renderQuad(int x, int y, int nWidth, int nHeight) ;
} ;