#include "D3D9RenderingManager.h"

CD3D9RenderingManager::CD3D9RenderingManager()
{
}
CD3D9RenderingManager::~CD3D9RenderingManager()
{
}
void CD3D9RenderingManager::initialize(LPDIRECT3DDEVICE9 pd3dDevice, int nMaxNumNode, CSecretCamera *pcCamera, CSecretFrustum *pcFrustum, float fCameraBoundingSphereRadius)
{
	CSecretRenderingManager::initialize(nMaxNumNode, pcCamera, pcFrustum, fCameraBoundingSphereRadius) ;
	m_pd3dDevice = pd3dDevice ;
}
void CD3D9RenderingManager::render(CSecretShader *pcShader, int nPass)
{
	DWORD dwBlend, dwSrcBlend, dwDestBlend ;

	//render opaque models
	m_pd3dDevice->GetRenderState(D3DRS_ALPHABLENDENABLE, &dwBlend) ;
	m_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE) ;

	m_psNodeContainers[MESHKIND_NOALPHA].render(pcShader, nPass, m_pfconditionMesh) ;


	m_pd3dDevice->GetRenderState(D3DRS_SRCBLEND, &dwSrcBlend) ;
	m_pd3dDevice->GetRenderState(D3DRS_DESTBLEND, &dwDestBlend) ;

	//render transparent models
	m_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE) ;
	m_pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA) ;
    m_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA) ;

	m_psNodeContainers[MESHKIND_ALPHA].render(pcShader, nPass, m_pfconditionMesh) ;

	m_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, dwBlend) ;
	m_pd3dDevice->SetRenderState(D3DRS_SRCBLEND, dwSrcBlend) ;
	m_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, dwDestBlend) ;
}
void CD3D9RenderingManager::renderOpaque(CSecretShader *pcShader, int nPass)
{
	DWORD dwBlend ;

	//render opaque models
	m_pd3dDevice->GetRenderState(D3DRS_ALPHABLENDENABLE, &dwBlend) ;
	m_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE) ;

	m_psNodeContainers[MESHKIND_NOALPHA].render(pcShader, nPass, m_pfconditionMesh) ;

	m_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, dwBlend) ;
}
void CD3D9RenderingManager::renderOpaqueRigid(CSecretShader *pcShader, int nPass)
{
	DWORD dwBlend ;

	//render opaque models
	m_pd3dDevice->GetRenderState(D3DRS_ALPHABLENDENABLE, &dwBlend) ;
	m_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE) ;

	m_psNodeContainers[MESHKIND_NOALPHA].renderRigid(pcShader, nPass, m_pfconditionMesh) ;

	m_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, dwBlend) ;
}
void CD3D9RenderingManager::renderOpaqueSkinned(CSecretShader *pcShader, int nPass)
{
	DWORD dwBlend ;

	//render opaque models
	m_pd3dDevice->GetRenderState(D3DRS_ALPHABLENDENABLE, &dwBlend) ;
	m_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE) ;

	m_psNodeContainers[MESHKIND_NOALPHA].renderSkinned(pcShader, nPass, m_pfconditionMesh) ;

	m_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, dwBlend) ;
}
void CD3D9RenderingManager::renderTransparent(CSecretShader *pcShader, int nPass)
{
	DWORD dwBlend, dwSrcBlend, dwDestBlend ;

	m_pd3dDevice->GetRenderState(D3DRS_ALPHABLENDENABLE, &dwBlend) ;
	m_pd3dDevice->GetRenderState(D3DRS_SRCBLEND, &dwSrcBlend) ;
	m_pd3dDevice->GetRenderState(D3DRS_DESTBLEND, &dwDestBlend) ;

	//render transparent models
	m_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE) ;
	m_pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA) ;
    m_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA) ;

	m_psNodeContainers[MESHKIND_ALPHA].render(pcShader, nPass, m_pfconditionMesh) ;

	m_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, dwBlend) ;
	m_pd3dDevice->SetRenderState(D3DRS_SRCBLEND, dwSrcBlend) ;
	m_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, dwDestBlend) ;
}