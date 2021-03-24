#include "D3D9PPLPPFinal.h"
#include "D3D9Shader.h"
#include "D3D9RenderTarget.h"
#include "HLSLLBuffer.h"

CD3D9PPLPPFinal::CD3D9PPLPPFinal()
{
}
CD3D9PPLPPFinal::~CD3D9PPLPPFinal()
{
}
HRESULT CD3D9PPLPPFinal::initialize(LPDIRECT3DDEVICE9 pd3dDevice, CD3D9RenderTarget *pcRenderTarget, CD3D9Shader *pcShader)
{
	CD3D9PostProcessing::initialize(pd3dDevice, pcRenderTarget, pcShader) ;

	m_pcShader->initializeParameters() ;

	return S_OK ;
}
void CD3D9PPLPPFinal::render(int nPass)
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

	m_pcShader->setTechnique() ;
	m_pcShader->begin() ;
	m_pcShader->beginPass(nPass) ;

	m_pcShader->setParametersToShader(NULL) ;

	m_pcShader->m_pEffect->CommitChanges() ;
	m_pd3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, m_sVertices, sizeof(SVertex)) ;

	m_pcShader->endPass() ;
	m_pcShader->end() ;

	m_pd3dDevice->SetRenderState(D3DRS_ZENABLE, dwZEnable) ;
	m_pd3dDevice->SetTextureStageState(0, D3DTSS_COLOROP, dwColorOp0) ;
	m_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG1, dwColorArg1) ;
	m_pd3dDevice->SetTextureStageState(1, D3DTSS_COLOROP, dwColorOp1) ;
}
