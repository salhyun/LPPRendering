#include "D3D9RenderTarget.h"
#include "def.h"

CD3D9RenderTarget::CD3D9RenderTarget()
{
	m_pd3dDevice = NULL ;
	m_pTex = NULL ;
	m_pSurf = NULL ;
	m_pSurfZBuffer = NULL ;
	m_nHeight = m_nWidth = 1024 ;
	m_fNear = 0.1f ;
	m_fFar = 1024.0f ;

	m_pOldSurfBack = NULL ;
	m_pOldSurfZBuffer = NULL ;
	ZeroMemory(&m_OldViewport, sizeof(D3DVIEWPORT9)) ;
	ZeroMemory(&m_Viewport, sizeof(D3DVIEWPORT9)) ;

	sprintf_s(m_szName, 256, "") ;
}
CD3D9RenderTarget::~CD3D9RenderTarget()
{
	release() ;
}
void CD3D9RenderTarget::release()
{
	SAFE_RELEASE(m_pSurfZBuffer) ;
	SAFE_RELEASE(m_pSurf) ;
	SAFE_RELEASE(m_pTex) ;
}
HRESULT CD3D9RenderTarget::initialize(LPDIRECT3DDEVICE9 pd3dDevice, int nWidth, int nHeight, D3DFORMAT d3dfmt_rgb, D3DFORMAT d3dfmt_depth, float fNear, float fFar)
{
	HRESULT hr ;
	m_pd3dDevice = pd3dDevice ;
	m_nWidth = nWidth ;
	m_nHeight = nHeight ;
	m_fNear = fNear ;
	m_fFar = fFar ;

	hr = m_pd3dDevice->CreateTexture(m_nWidth, m_nHeight, 1, D3DUSAGE_RENDERTARGET, d3dfmt_rgb, D3DPOOL_DEFAULT, &m_pTex, NULL) ;
	RETURN_D3D9FAILED(hr) ;

	hr = m_pTex->GetSurfaceLevel(0, &m_pSurf) ;
	RETURN_D3D9FAILED(hr) ;

	m_pd3dDevice->ColorFill(m_pSurf, NULL, 0) ;

	hr = m_pd3dDevice->CreateDepthStencilSurface(m_nWidth, m_nHeight, d3dfmt_depth, D3DMULTISAMPLE_NONE, 0, TRUE, &m_pSurfZBuffer, 0) ;
	RETURN_D3D9FAILED(hr) ;

	m_Viewport.X = 0 ;
	m_Viewport.Y = 0 ;
	m_Viewport.Width = m_nWidth ;
	m_Viewport.Height = m_nHeight ;
	m_Viewport.MinZ = 0.0f ;
	m_Viewport.MaxZ = 1.0f ;

	return S_OK ;
}
void CD3D9RenderTarget::storeBackBuffer()
{
	m_pd3dDevice->GetRenderTarget(0, &m_pOldSurfBack) ;
	m_pd3dDevice->GetDepthStencilSurface(&m_pOldSurfZBuffer) ;
	m_pd3dDevice->GetViewport(&m_OldViewport) ;
}
void CD3D9RenderTarget::setRenderTarget()
{
	m_pd3dDevice->SetViewport(&m_Viewport) ;
	m_pd3dDevice->SetRenderTarget(0, m_pSurf) ;
	m_pd3dDevice->SetDepthStencilSurface(m_pSurfZBuffer) ;
}
void CD3D9RenderTarget::restoreBackbuffer()
{
	m_pd3dDevice->SetDepthStencilSurface(m_pOldSurfZBuffer) ;
	m_pd3dDevice->SetRenderTarget(0, m_pOldSurfBack) ;
	m_pd3dDevice->SetViewport(&m_OldViewport) ;

	SAFE_RELEASE(m_pOldSurfZBuffer) ;
	SAFE_RELEASE(m_pOldSurfBack) ;
}
void CD3D9RenderTarget::renderQuad(int x, int y, int nWidth, int nHeight)
{
	D3DVIEWPORT9 viewport ;
	m_pd3dDevice->GetViewport(&viewport) ;
	float fMagicNumber = -0.5f ;

	float fVertices[] =
	{
		(float)x,			(float)y,			0.0f,			1.0f,			0.0f,			0.0f,
		(float)(x+nWidth),	(float)y,			0.0f,			1.0f,			1.0f,			0.0f,
		(float)(x+nWidth),  (float)(y+nHeight),	0.0f,			1.0f,			1.0f,			1.0f,
		(float)x,			(float)(y+nHeight), 0.0f,			1.0f,			0.0f,			1.0f
	} ;

	fVertices[0] += fMagicNumber ;
	fVertices[1] += fMagicNumber ;

	fVertices[6] += fMagicNumber ;
	fVertices[7] += fMagicNumber ;

	fVertices[12] += fMagicNumber ;
	fVertices[13] += fMagicNumber ;

	fVertices[18] += fMagicNumber ;
	fVertices[19] += fMagicNumber ;

	m_pd3dDevice->SetTexture(0, m_pTex) ;
	m_pd3dDevice->SetFVF( D3DFVF_XYZRHW | D3DFVF_TEX1 );
	m_pd3dDevice->SetPixelShader(0);
	m_pd3dDevice->DrawPrimitiveUP( D3DPT_TRIANGLEFAN, 2, fVertices, sizeof(float)*6 );
	m_pd3dDevice->SetTexture( 0,  NULL);
}
