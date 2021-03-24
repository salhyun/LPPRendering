#include "D3D9PPLBuffer.h"
#include "D3D9Shader.h"
#include "D3D9RenderTarget.h"
#include "HLSLLBuffer.h"

CD3D9PPLBuffer::CD3D9PPLBuffer()
{
	m_pTexNormal = NULL ;
	m_pTexDepth = NULL ;
	m_pTexSpecularParam = NULL ;
	m_pTexPrev = NULL ;
}
CD3D9PPLBuffer::~CD3D9PPLBuffer()
{
}
HRESULT CD3D9PPLBuffer::initialize(LPDIRECT3DDEVICE9 pd3dDevice, CD3D9RenderTarget *pcRenderTarget, CD3D9Shader *pcShader)
{
	CD3D9PostProcessing::initialize(pd3dDevice, pcRenderTarget, pcShader) ;

	m_pcShader->initializeParameters() ;

	return S_OK ;
}
void CD3D9PPLBuffer::render(int nPass)
{
	DWORD dwZEnable, dwColorOp0, dwColorOp1, dwColorArg1 ;
	//backup old
	m_pd3dDevice->GetRenderState(D3DRS_ZENABLE, &dwZEnable) ;
	m_pd3dDevice->GetTextureStageState(0, D3DTSS_COLOROP, &dwColorOp0) ;
	m_pd3dDevice->GetTextureStageState(0, D3DTSS_COLORARG1, &dwColorArg1) ;
	m_pd3dDevice->GetTextureStageState(1, D3DTSS_COLOROP, &dwColorOp1) ;

	m_pd3dDevice->SetRenderState(D3DRS_ZENABLE, FALSE) ;
	m_pd3dDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1) ;
	m_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE) ;
	m_pd3dDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE) ;
	m_pd3dDevice->SetFVF(D3DFVF_POSTPROCESSINGVERTEX) ;

	m_pcRenderTarget->storeBackBuffer() ;
	m_pcRenderTarget->setRenderTarget() ;

	m_pcShader->setTechnique() ;
	m_pcShader->begin() ;
	m_pcShader->beginPass(nPass) ;

	DWORD dwAlphaBlendEnable, dwSrcBlend, dwDestBlend, dwBlendOp, dwAlphaFunc, dwAlphaRef;

	//backup old
	m_pd3dDevice->GetRenderState(D3DRS_ALPHABLENDENABLE, &dwAlphaBlendEnable);
	m_pd3dDevice->GetRenderState(D3DRS_SRCBLEND, &dwSrcBlend);
	m_pd3dDevice->GetRenderState(D3DRS_DESTBLEND, &dwDestBlend);
	m_pd3dDevice->GetRenderState(D3DRS_BLENDOP, &dwBlendOp);
	m_pd3dDevice->GetRenderState(D3DRS_ALPHAFUNC, &dwAlphaFunc);
	m_pd3dDevice->GetRenderState(D3DRS_ALPHAREF, &dwAlphaRef);

	m_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);//light buffer
	m_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);//back buffer
	m_pd3dDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);

	//m_pd3dDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	//m_pd3dDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	m_pcShader->setParametersToShader(NULL) ;
	m_pcShader->m_pEffect->SetTexture("NormalMap", m_pTexNormal) ;
	m_pcShader->m_pEffect->SetTexture("DepthMap", m_pTexDepth) ;
	m_pcShader->m_pEffect->SetTexture("SpecularParamMap", m_pTexSpecularParam) ;

	m_pcShader->m_pEffect->CommitChanges() ;
	m_pd3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, m_sVertices, sizeof(SVertex)) ;

	m_pcShader->endPass() ;
	m_pcShader->end() ;

	m_pcRenderTarget->restoreBackbuffer() ;

	//restore old
	m_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, dwAlphaBlendEnable);
	m_pd3dDevice->SetRenderState(D3DRS_SRCBLEND, dwSrcBlend);
	m_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, dwDestBlend);
	m_pd3dDevice->SetRenderState(D3DRS_BLENDOP, dwBlendOp);
	m_pd3dDevice->SetRenderState(D3DRS_ALPHAFUNC, dwAlphaFunc);
	m_pd3dDevice->SetRenderState(D3DRS_ALPHAREF, dwAlphaRef);

	//restore old
	m_pd3dDevice->SetRenderState(D3DRS_ZENABLE, dwZEnable) ;
	m_pd3dDevice->SetTextureStageState(0, D3DTSS_COLOROP, dwColorOp0) ;
	m_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG1, dwColorArg1) ;
	m_pd3dDevice->SetTextureStageState(1, D3DTSS_COLOROP, dwColorOp1) ;
}
