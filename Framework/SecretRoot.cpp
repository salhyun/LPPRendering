#include "SecretRoot.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <time.h>

#include "D3D9Include.h"
#include "Secret.h"
#include "SecretInput.h"
#include "TestScene.h"
#include "../SecretTimer.h"
#include "HLSLRigid.h"
#include "HLSLLPPOpaque.h"
#include "HLSLSSAO.h"
#include "HLSLGBuffer.h"
#include "HLSLLBuffer.h"
#include "HLSLLPPFinal.h"
#include "HLSLTSM.h"

ImplementDynamic(CSecretRoot)
ImplementDynCreate(CSecretRoot)

CSecretRoot::CSecretRoot()
{
	m_pcEnvironment = NULL ;
	m_pcShaderManager = NULL ;
	m_pcCamera = NULL ;
	m_pcInput = NULL ;
	m_pcTimer = NULL ;

	m_bTerminator = false ;

	for(int i=0 ; i<MAXNUM_SCENE ; i++)
		m_apcScene[i] = NULL ;
}

CSecretRoot::~CSecretRoot()
{
	Release() ;
}
void CSecretRoot::Release()
{
	for(int i=0 ; i<MAXNUM_SCENE ; i++)
	{
		if(m_apcScene[i])
		{
			m_apcScene[i]->Release() ;
			delete m_apcScene[i] ;
			m_apcScene[i] = NULL ;
		}
	}

	SAFE_DELETE(m_pcShaderManager) ;
	SAFE_DELETE(m_pcRenderingIngredient) ;
	SAFE_DELETE(m_pcCamera) ;
	SAFE_DELETE(m_pcInput) ;
	SAFE_DELETE(m_pcTimer) ;
}
void CSecretRoot::_initShader()
{
	m_pcRenderingIngredient = new CRenderingIngredient() ;

	m_pcShaderManager = new CD3D9ShaderManager() ;
	m_pcShaderManager->initialize() ;

	CHLSLRigid *rigid = new CHLSLRigid() ;
	rigid->initialize(m_pcEnvironment->getD3DDevice(), "shaders/rigid.fx") ;
	rigid->m_pcIngredient = m_pcRenderingIngredient ;
	m_pcShaderManager->addShader((CD3D9Shader *)rigid) ;

	CHLSLLPPOpaque *opaque = new CHLSLLPPOpaque() ;
	opaque->initialize(m_pcEnvironment->getD3DDevice(), "shaders/LPPopaque.fx") ;
	opaque->m_pcIngredient = m_pcRenderingIngredient ;
	m_pcShaderManager->addShader((CD3D9Shader *)opaque) ;

	CHLSLGBuffer *gbuffer = new CHLSLGBuffer() ;
	gbuffer->initialize(m_pcEnvironment->getD3DDevice(), "shaders/gbuffer.fx") ;
	gbuffer->m_pcIngredient = m_pcRenderingIngredient ;
	m_pcShaderManager->addShader((CD3D9Shader *)gbuffer) ;

	CHLSLSSAO *ssao = new CHLSLSSAO() ;
	ssao->initialize(m_pcEnvironment->getD3DDevice(), "shaders/SSAO.fx") ;
	ssao->m_pcIngredient = m_pcRenderingIngredient ;
	m_pcShaderManager->addShader((CD3D9Shader *)ssao) ;

	CHLSLLBuffer *lbuffer = new CHLSLLBuffer() ;
	lbuffer->initialize(m_pcEnvironment->getD3DDevice(), "shaders/lbuffer.fx") ;
	lbuffer->m_pcIngredient = m_pcRenderingIngredient ;
	m_pcShaderManager->addShader((CD3D9Shader *)lbuffer) ;

	//CHLSLLPPFinal *final = new CHLSLLPPFinal() ;
	//final->initialize(m_pcEnvironment->getD3DDevice(), "shaders/LPPFinal.fx") ;
	//final->m_pcIngredient = m_pcRenderingIngredient ;
	//m_pcShaderManager->addShader((CD3D9Shader *)final) ;

	CHLSLTSM *tsm = new CHLSLTSM() ;
	tsm->initialize(m_pcEnvironment->getD3DDevice(), "shaders/tsm.fx") ;
	tsm->m_pcIngredient = m_pcRenderingIngredient ;
	m_pcShaderManager->addShader((CD3D9Shader *)tsm) ;

}
bool CSecretRoot::Initialize(CD3D9Environment *pcEnvironment)
{
    RTClass *pRTClass ;
	int nSceneCount=0 ;

	m_pcTimer = new CSecretTimer() ;
	m_pcTimer->Initialize_real() ;

	m_pcInput = new CSecretInput() ;

	m_pcEnvironment = pcEnvironment ;
	_initShader() ;

	m_pcCamera = new CSecretCamera() ;

	//pRTClass = RuntimeClass(CGameTitle) ;
	//m_apcScene[nSceneCount++] = (CSecretScene *)(pRTClass->CreateSecretObject()) ;

	pRTClass = RuntimeClass(CTestScene) ;
	m_apcScene[nSceneCount] = (CSecretScene *)(pRTClass->CreateSecretObject()) ;
	m_apcScene[nSceneCount]->setRoot(this) ;
	nSceneCount++ ;

	m_nCurScene = SCENE_TESTSCENE ;

	m_pcCurScene = m_apcScene[m_nCurScene] ;
	SetSceneStatus(ROOTSTA_SCENEINIT) ;

    return true ;
}

void CSecretRoot::Process()
{
	switch(m_nSceneStatus)
	{
	case ROOTSTA_SCENEINIT :// Scene Initialize
		m_pcCurScene->Initialize() ;
		SetSceneStatus(ROOTSTA_SCENEPREPROC) ;
		break ;

	case ROOTSTA_SCENEPREPROC ://PreProcessing Scene before SceneProcess and Rendering
		m_pcCurScene->PreProcess() ;
		SetSceneStatus(ROOTSTA_SCENEPROC) ;
		break ;

	case ROOTSTA_SCENEPROC :// Scene Process and Rendering
		m_pcCurScene->Process() ;
		break ;

	case ROOTSTA_SCENEPOSTPROC :
		m_pcCurScene->PostProcess() ;
		SetSceneStatus(ROOTSTA_SCENECHANGE) ;
		break ;

	case ROOTSTA_SCENECHANGE :// Scene Release and Change Next Scene
		m_pcCurScene->Release() ;
		m_nCurScene = m_nNextScene ;
		m_pcCurScene = m_apcScene[m_nCurScene] ;
		SetSceneStatus(ROOTSTA_SCENEINIT) ;
		break ;
	} ;
}
void CSecretRoot::SetSceneStatus(int nStatus)
{
	m_nSceneStatus = nStatus ;
}
unsigned long CSecretRoot::GetSceneStatus()
{
	return m_nSceneStatus ;
}
void CSecretRoot::Render()
{
	if(GetSceneStatus() == ROOTSTA_SCENEPROC)
		m_pcCurScene->Render() ;
}
CSecretScene *CSecretRoot::getCurrentScene()
{
	return m_pcCurScene ;
}
CD3D9Environment *CSecretRoot::getEnvironment()
{
	return m_pcEnvironment ;
}
CRenderingIngredient *CSecretRoot::getRenderingIngredient()
{
	return m_pcRenderingIngredient ;
}
CD3D9ShaderManager *CSecretRoot::getShaderManager()
{
	return m_pcShaderManager ;
}
CSecretCamera *CSecretRoot::getCamera()
{
	return m_pcCamera ;
}
