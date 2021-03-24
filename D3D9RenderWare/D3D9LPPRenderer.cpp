#include "D3D9LPPRenderer.h"
#include "SecretLightProperty.h"
#include "def.h"
#include "D3D9Environment.h"
#include "D3D9ShaderManager.h"
#include "D3D9RenderTarget.h"
#include "D3D9MultipleRenderTargets.h"
#include "HLSLLBuffer.h"
#include "HLSLLPPFinal.h"
#include "D3D9PPLBuffer.h"
#include "D3D9PPLPPFinal.h"
#include "SecretNode.h"

CD3D9LPPRenderer::CD3D9LPPRenderer()
{
	m_pd3dDevice = NULL ;
	m_pcGBufferMRTs = NULL ;
	m_pcLBuffer = NULL ;
	m_cLightProperties.resize(0) ;
}
CD3D9LPPRenderer::~CD3D9LPPRenderer()
{
	release() ;
}
void CD3D9LPPRenderer::release()
{
	std::vector<CSecretLightProperty *>::iterator it ;
	for(it=m_cLightProperties.begin(); it!=m_cLightProperties.end(); it++)
		delete (*it) ;

	SAFE_DELETE(m_pcGBufferMRTs) ;

	SAFE_DELETE(m_pcLBuffer) ;
}
void CD3D9LPPRenderer::initialize(CD3D9Environment *pcEnvironment, CD3D9ShaderManager *pcShaderManager)
{
	m_pd3dDevice = pcEnvironment->getD3DDevice() ;
	D3DPRESENT_PARAMETERS *parameters = pcEnvironment->getPresentParameters() ;

	m_pcShaderManager = pcShaderManager ;

	//initialize multi render targets
	m_pcGBufferMRTs = new CD3D9MultipleRenderTargets() ;
	m_pcGBufferMRTs->initialize(pcEnvironment->getD3DDevice(), pcEnvironment->getDirect3d(), pcEnvironment->m_lScreenWidth, pcEnvironment->m_lScreenHeight) ;
	//m_pcGBufferMRTs->addRenderTarget(D3DFMT_A8R8G8B8) ;//light color
	m_pcGBufferMRTs->addRenderTarget(D3DFMT_A16B16G16R16F);//light color
	m_pcGBufferMRTs->addRenderTarget(D3DFMT_R32F) ;//depth
	m_pcGBufferMRTs->addRenderTarget(D3DFMT_A8R8G8B8) ;//shine

	//initialize light buffers
	CD3D9RenderTarget *pcRTLBuffer ;
	pcRTLBuffer = new CD3D9RenderTarget() ;
	pcRTLBuffer->initialize(
		pcEnvironment->getD3DDevice(),
		pcEnvironment->m_lScreenWidth,
		pcEnvironment->m_lScreenHeight,
		parameters->BackBufferFormat,//D3DFMT_A32B32G32R32F,//parameters->BackBufferFormat,
		parameters->AutoDepthStencilFormat,
		pcEnvironment->m_fNear,
		pcEnvironment->m_fFar
		) ;

	m_pcLBuffer = new CD3D9PPLBuffer() ;
	m_pcLBuffer->initialize(pcEnvironment->getD3DDevice(), pcRTLBuffer, m_pcShaderManager->findShader("lbuffer")) ;
	
	m_pcRenderingIngredient = m_pcShaderManager->findShader("lbuffer")->getIngredient() ;
}
void CD3D9LPPRenderer::storeBackBuffer()
{
	m_pcGBufferMRTs->storeBackBuffer() ;
}
void CD3D9LPPRenderer::setGBuffer()
{
	m_pcGBufferMRTs->setRenderTarget() ;
	m_pd3dDevice->Clear(0L, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, 0x00000000, 1.0f, 0L) ;
}
void CD3D9LPPRenderer::restoreBackBuffer()
{
	m_pcGBufferMRTs->restoreBackbuffer() ;
}
void CD3D9LPPRenderer::_setParameters(CHLSLLBuffer *pcLBuffer, CSecretLightProperty *pcLightProperty)
{
	if(pcLightProperty->isType(CSecretLightProperty::LIGHTTYPE_POINT))
	{
		pcLBuffer->m_vLightPos = D3DXVECTOR3(pcLightProperty->m_vLightPos.x, pcLightProperty->m_vLightPos.y, pcLightProperty->m_vLightPos.z) ;
		pcLBuffer->m_vLightColor = D3DXVECTOR3(pcLightProperty->m_vLightColor.x, pcLightProperty->m_vLightColor.y, pcLightProperty->m_vLightColor.z) ;
		pcLBuffer->m_fLightRange = pcLightProperty->m_fLightRange ;
	}
	else if(pcLightProperty->isType(CSecretLightProperty::LIGHTTYPE_DIRECTIONAL))
	{
		pcLBuffer->m_vLightDir = D3DXVECTOR3(pcLightProperty->m_vLightDir.x, pcLightProperty->m_vLightDir.y, pcLightProperty->m_vLightDir.z) ;
		pcLBuffer->m_vLightColor = D3DXVECTOR3(pcLightProperty->m_vLightColor.x, pcLightProperty->m_vLightColor.y, pcLightProperty->m_vLightColor.z) ;
	}
}
void CD3D9LPPRenderer::render()
{
	m_pcLBuffer->m_pTexNormal = m_pcGBufferMRTs->getTexture(0) ;
	m_pcLBuffer->m_pTexDepth = m_pcGBufferMRTs->getTexture(1) ;
	m_pcLBuffer->m_pTexSpecularParam = m_pcGBufferMRTs->getTexture(2) ;
	D3DXMATRIX matInvProj ;
	D3DXMatrixInverse(&matInvProj, NULL, &m_pcRenderingIngredient->m_matProj) ;

	D3DXMATRIX matInvVP ;
	D3DXMATRIX matVP = m_pcRenderingIngredient->m_matView * m_pcRenderingIngredient->m_matProj ;
	D3DXMatrixInverse(&matInvVP, NULL, &matVP) ;

	m_pd3dDevice->Clear(0L, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0, 1.0f, 0L);
	m_pd3dDevice->ColorFill(m_pcLBuffer->getRenderTarget()->m_pSurf, NULL, 0);
	//m_pd3dDevice->ColorFill(m_pcLBuffer->getRenderTarget()->m_pSurf, NULL, 0x00ffffff) ;

	CHLSLLBuffer *lbuffer ;
	std::vector<CSecretLightProperty *>::iterator it ;
	for(it=m_cLightProperties.begin(); it!=m_cLightProperties.end(); it++)
	{
		m_pcLBuffer->m_pTexNormal = m_pcGBufferMRTs->getTexture(0) ;
		m_pcLBuffer->m_pTexDepth = m_pcGBufferMRTs->getTexture(1) ;
		m_pcLBuffer->m_pTexSpecularParam = m_pcGBufferMRTs->getTexture(2) ;

		lbuffer = (CHLSLLBuffer *)m_pcLBuffer->getShader() ;
		lbuffer->m_matInvVP = matInvVP ;
		lbuffer->m_vCameraPos = m_pcRenderingIngredient->m_vCameraPos ;
		_setParameters(lbuffer, (*it)) ;

		if((*it)->isType(CSecretLightProperty::LIGHTTYPE_DIRECTIONAL))
			m_pcLBuffer->render(0) ;
		if((*it)->isType(CSecretLightProperty::LIGHTTYPE_POINT))
			m_pcLBuffer->render(1) ;
	}
}
void CD3D9LPPRenderer::renderQuad(int x, int y, int width, int height)
{
	m_pcLBuffer->getRenderTarget()->renderQuad(x, y, width, height) ;
}
void CD3D9LPPRenderer::addLightProperty(CSecretLightProperty *pcLightProperty)
{
	m_cLightProperties.push_back(pcLightProperty) ;
}
CD3D9PPLBuffer *CD3D9LPPRenderer::getCurrentLBuffer()
{
	return m_pcLBuffer ;
}
void CD3D9LPPRenderer::findGBufferShader(int nNodeKind, CSecretShader **shader, int *pass)
{
	if(nNodeKind == SECRETNODE_KIND_RIGID)
	{
		*shader = (CSecretShader *)m_pcShaderManager->findShader("gbuffer") ;
		*pass = 0 ;
	}
	else if(nNodeKind == SECRETNODE_KIND_RIGIDBUMP)
	{
		*shader = (CSecretShader *)m_pcShaderManager->findShader("gbuffer") ;
		*pass = 1 ;
	}
}
CD3D9MultipleRenderTargets *CD3D9LPPRenderer::getGBufferMRTs()
{
	return m_pcGBufferMRTs ;
}
std::vector<CSecretLightProperty *> *CD3D9LPPRenderer::getLightProperites() {
	return &m_cLightProperties;
}