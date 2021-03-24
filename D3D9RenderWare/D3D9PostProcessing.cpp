#include "D3D9PostProcessing.h"
#include "def.h"
#include "D3D9RenderTarget.h"

CD3D9PostProcessing::CD3D9PostProcessing()
{
	m_pd3dDevice = NULL ;
	m_pcRenderTarget = NULL ;
	m_pcShader = NULL ;
}
CD3D9PostProcessing::~CD3D9PostProcessing()
{
	release() ;
}
void CD3D9PostProcessing::release()
{
	SAFE_DELETE(m_pcRenderTarget) ;
}
HRESULT CD3D9PostProcessing::initialize(LPDIRECT3DDEVICE9 pd3dDevice, CD3D9RenderTarget *pcRenderTarget, CD3D9Shader *pcShader)
{
	m_pd3dDevice = pd3dDevice ;
	m_pcRenderTarget = pcRenderTarget ;
	m_pcShader = pcShader ;

	m_sVertices[0].pos = D3DXVECTOR3(-1.0f, +1.0f, 0.1f) ;
	m_sVertices[0].tex = D3DXVECTOR2(0, 0) ;
	m_sVertices[1].pos = D3DXVECTOR3(+1.0f, +1.0f, 0.1f) ;
	m_sVertices[1].tex = D3DXVECTOR2(1, 0) ;
	m_sVertices[2].pos = D3DXVECTOR3(+1.0f, -1.0f, 0.1f) ;
	m_sVertices[2].tex = D3DXVECTOR2(1, 1) ;
	m_sVertices[3].pos = D3DXVECTOR3(-1.0f, -1.0f, 0.1f) ;
	m_sVertices[3].tex = D3DXVECTOR2(0, 1) ;

	return S_OK ;
}
void CD3D9PostProcessing::render(int nPass)
{
}
CD3D9RenderTarget *CD3D9PostProcessing::getRenderTarget()
{
	return m_pcRenderTarget ;
}
CD3D9Shader *CD3D9PostProcessing::getShader()
{
	return m_pcShader ;
}
