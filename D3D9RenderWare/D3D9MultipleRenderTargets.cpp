#include "D3D9MultipleRenderTargets.h"
#include "D3D9RenderTarget.h"
#include "def.h"

CD3D9MultipleRenderTargets::SRenderTarget::SRenderTarget()
{
	m_pTex = NULL ;
	m_pSurf = NULL ;
}
CD3D9MultipleRenderTargets::SRenderTarget::~SRenderTarget()
{
	release() ;
}
void CD3D9MultipleRenderTargets::SRenderTarget::release()
{
	SAFE_RELEASE(m_pSurf) ;
	SAFE_RELEASE(m_pTex) ;
}

CD3D9MultipleRenderTargets::CD3D9MultipleRenderTargets()
{
	m_pSurfZBuffer = NULL ;
}
CD3D9MultipleRenderTargets::~CD3D9MultipleRenderTargets()
{
	release() ;
}
void CD3D9MultipleRenderTargets::release()
{
	std::vector<SRenderTarget *>::iterator it ;
	for(it=m_cRenderTargets.begin(); it!=m_cRenderTargets.end(); it++)
		delete (*it) ;

	SAFE_RELEASE(m_pSurfZBuffer) ;
}
HRESULT CD3D9MultipleRenderTargets::initialize(LPDIRECT3DDEVICE9 pd3dDevice, LPDIRECT3D9 pd3d9, int nWidth, int nHeight, D3DFORMAT d3dfmt_depth)
{
	HRESULT hr ;
	m_pd3dDevice = pd3dDevice ;
	m_cRenderTargets.resize(0) ;

	m_nWidth = nWidth ;
	m_nHeight = nHeight ;

	D3DMULTISAMPLE_TYPE multisampleType = D3DMULTISAMPLE_NONE;
	//multisampleType = D3DMULTISAMPLE_TYPE::D3DMULTISAMPLE_8_SAMPLES;

	hr = m_pd3dDevice->CreateDepthStencilSurface(m_nWidth, m_nHeight, d3dfmt_depth, multisampleType, 0, TRUE, &m_pSurfZBuffer, 0) ;
	RETURN_D3D9FAILED(hr) ;

	m_Viewport.X = 0 ;
	m_Viewport.Y = 0 ;
	m_Viewport.Width = m_nWidth ;
	m_Viewport.Height = m_nHeight ;
	m_Viewport.MinZ = 0.0f ;
	m_Viewport.MaxZ = 1.0f ;

	return S_OK ;
}
HRESULT CD3D9MultipleRenderTargets::addRenderTarget(D3DFORMAT d3dfmt_rgb)
{
	HRESULT hr ;
	SRenderTarget *psRenderTarget = new SRenderTarget() ;

	hr = m_pd3dDevice->CreateTexture(m_nWidth, m_nHeight, 1, D3DUSAGE_RENDERTARGET, d3dfmt_rgb, D3DPOOL_DEFAULT, &psRenderTarget->m_pTex, NULL) ;
	RETURN_D3D9FAILED(hr) ;

	hr = psRenderTarget->m_pTex->GetSurfaceLevel(0, &psRenderTarget->m_pSurf) ;
	RETURN_D3D9FAILED(hr) ;

	m_pd3dDevice->ColorFill(psRenderTarget->m_pSurf, NULL, 0) ;

	m_cRenderTargets.push_back(psRenderTarget) ;

	return S_OK ;
}
CD3D9MultipleRenderTargets::SRenderTarget *CD3D9MultipleRenderTargets::getRenderTarget(int i)
{
	return m_cRenderTargets.at(i) ;
}
LPDIRECT3DTEXTURE9 CD3D9MultipleRenderTargets::getTexture(int i)
{
	return m_cRenderTargets.at(i)->m_pTex ;
}
void CD3D9MultipleRenderTargets::storeBackBuffer()
{
	m_pd3dDevice->GetRenderTarget(0, &m_pOldSurfBack) ;
	m_pd3dDevice->GetDepthStencilSurface(&m_pOldSurfZBuffer) ;
	m_pd3dDevice->GetViewport(&m_OldViewport) ;
}
void CD3D9MultipleRenderTargets::setRenderTarget()
{
	m_pd3dDevice->SetViewport(&m_Viewport) ;

	int i=0 ;
	std::vector<SRenderTarget *>::iterator it ;
	for (it = m_cRenderTargets.begin(); it != m_cRenderTargets.end(); it++, i++) {
		m_pd3dDevice->SetRenderTarget(i, (*it)->m_pSurf);
	}
	m_pd3dDevice->SetDepthStencilSurface(m_pSurfZBuffer) ;
}
void CD3D9MultipleRenderTargets::restoreBackbuffer()
{
	m_pd3dDevice->SetDepthStencilSurface(m_pOldSurfZBuffer) ;

	m_pd3dDevice->SetRenderTarget(0, m_pOldSurfBack) ;
	for(int i=1; i<m_cRenderTargets.size(); i++)
		m_pd3dDevice->SetRenderTarget(i, NULL) ;

	m_pd3dDevice->SetViewport(&m_OldViewport) ;

	SAFE_RELEASE(m_pOldSurfZBuffer) ;
	SAFE_RELEASE(m_pOldSurfBack) ;
}
void CD3D9MultipleRenderTargets::renderQuad(int nNumRT, int x, int y, int nWidth, int nHeight)
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

	m_pd3dDevice->SetTexture(0, getTexture(nNumRT)) ;
	m_pd3dDevice->SetFVF( D3DFVF_XYZRHW | D3DFVF_TEX1 );
	m_pd3dDevice->SetPixelShader(0);
	m_pd3dDevice->DrawPrimitiveUP( D3DPT_TRIANGLEFAN, 2, fVertices, sizeof(float)*6 );
	m_pd3dDevice->SetTexture( 0,  NULL);
}
