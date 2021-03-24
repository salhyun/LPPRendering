#include "D3D9PPSSAO.h"
#include "D3D9Shader.h"
#include "D3D9RenderTarget.h"
#include "HLSLSSAO.h"
#include "D3D9Environment.h"

CD3D9PPSSAO::CD3D9PPSSAO()
{
	m_pTexNormal = NULL ;
	m_pTexDepth = NULL ;
	m_pTexRandom = NULL ;
	m_pcRTSSAO = NULL;
}
CD3D9PPSSAO::~CD3D9PPSSAO()
{
}
void CD3D9PPSSAO::setSampleOffsets()
{
	CHLSLSSAO *ssao = (CHLSLSSAO *)m_pcShader ;

	float w = 1.0f/m_pcRenderTarget->m_nWidth ;
	float h = 1.0f/m_pcRenderTarget->m_nHeight ;

	for(int i=0; i<ssao->MAX_SAMPLES; i++)
	{
		ssao->m_pvSampleOffsets[i].x *= w ;
		ssao->m_pvSampleOffsets[i].y *= h ;
	}
}
void CD3D9PPSSAO::setMatInvProj(D3DXMATRIX *pmat)
{
	CHLSLSSAO *ssao = (CHLSLSSAO *)m_pcShader ;
	ssao->m_matInvProj = (*pmat) ;
}
HRESULT CD3D9PPSSAO::initialize(CD3D9Environment *pcEnvironment, CD3D9Shader *pcShader)
{
	D3DPRESENT_PARAMETERS *parameters = pcEnvironment->getPresentParameters();

	m_pcRTSSAO = new CD3D9RenderTarget();
	m_pcRTSSAO->initialize(
		pcEnvironment->getD3DDevice(),
		pcEnvironment->m_lScreenWidth,
		pcEnvironment->m_lScreenHeight,
		parameters->BackBufferFormat,//D3DFMT_A32B32G32R32F,//parameters->BackBufferFormat,
		parameters->AutoDepthStencilFormat,
		pcEnvironment->m_fNear,
		pcEnvironment->m_fFar
	);
	CD3D9PostProcessing::initialize(pcEnvironment->getD3DDevice(), m_pcRTSSAO, pcShader);
	return S_OK;
}

//HRESULT CD3D9PPSSAO::initialize(LPDIRECT3DDEVICE9 pd3dDevice, CD3D9RenderTarget *pcRenderTarget, CD3D9Shader *pcShader)
//{
//	CD3D9PostProcessing::initialize(pd3dDevice, pcRenderTarget, pcShader) ;
//
//	m_pcShader->initializeParameters() ;
//
//	return S_OK ;
//}
void CD3D9PPSSAO::render(int nPass)
{
	DWORD dwZEnable, dwColorOp0, dwColorOp1, dwColorArg1 ;

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

	m_pcShader->setParametersToShader(NULL) ;
	m_pcShader->m_pEffect->SetTexture("NormalMap", m_pTexNormal) ;
	m_pcShader->m_pEffect->SetTexture("DepthMap", m_pTexDepth) ;
	m_pcShader->m_pEffect->SetTexture("RandomMap", m_pTexRandom) ;

	m_pcShader->m_pEffect->CommitChanges() ;
	m_pd3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, m_sVertices, sizeof(SVertex)) ;

	m_pcShader->endPass() ;
	m_pcShader->end() ;

	m_pcRenderTarget->restoreBackbuffer() ;

	m_pd3dDevice->SetRenderState(D3DRS_ZENABLE, dwZEnable) ;
	m_pd3dDevice->SetTextureStageState(0, D3DTSS_COLOROP, dwColorOp0) ;
	m_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG1, dwColorArg1) ;
	m_pd3dDevice->SetTextureStageState(1, D3DTSS_COLOROP, dwColorOp1) ;
}
