#include "TestScene.h"
#include "SecretInput.h"
#include "SecretRoot.h"
#include "HLSLSSAO.h"
#include "D3D9PPSSAO.h"
#include "HLSLLBuffer.h"
#include "HLSLLPPOpaque.h"
#include "HLSLTSM.h"
#include "HLSLRigid.h"
#include "D3D9PPLBuffer.h"
#include "../d3dfont.h"
#include "../SecretTimer.h"

#include "Matrix.h"

#include "../VertexAnimationMesh.h"
#include <functional>


CVertexAnimationMesh cVertexAni ;

ImplementDynamic(CTestScene)
ImplementDynCreate(CTestScene)

CTestScene::SInput::SInput()
{
	bDrawFrustum = false ;
	bUpdateFrustum = false ;
}
CTestScene::SInput::~SInput()
{
}

CTestScene::CTestScene()
{
	m_pcTexContainer = NULL ;
	m_pcModels = NULL ;
	m_pcTNRModels = NULL ;

	m_pcRenderingManager = NULL ;

	m_pcRenderingIngredient = NULL ;
	m_pcShaderManager = NULL ;
	m_pcCamera = NULL ;
	m_pcLPPRenderer = NULL ;
	m_pcSSAO = NULL;

	m_pcd3dFont = NULL ;
	m_pcTimer = NULL ;

	m_pcCameraTracker = NULL ;

	//m_nRenderOption = FORWARD_RENDERING ;
	m_nRenderOption = LPP_RENDERING ;
	//m_nRenderOption |= FRUSTUM_RENDERING ;

	m_fShadowMapBias = -0.00258265f ;
}
CTestScene::~CTestScene()
{
	Release() ;
}
void CTestScene::Release()
{
	SAFE_DELETE(m_pcd3dFont) ;

	SAFE_DELETE(m_pcCamera) ;

	SAFE_DELETE(m_pcTexContainer) ;

	SAFE_DELETE(m_pcLPPRenderer) ;

	SAFE_DELETE(m_pcSSAO);

	SAFE_DELETE(m_pcCameraTracker) ;

	SAFE_DELETE(m_pcRenderingManager) ;

	if(m_pcModels)
	{
		std::vector<CD3D9Model *>::iterator it ;
		for(it=m_pcModels->begin(); it!=m_pcModels->end(); it++)
			delete (*it) ;
		delete m_pcModels ;
		m_pcModels = NULL ;
	}

	if(m_pcTNRModels)
	{
		std::vector<CSecretTNRModel *>::iterator it ;
		for(it=m_pcTNRModels->begin() ; it!=m_pcTNRModels->end() ; it++)
			delete (*it) ;
		delete m_pcTNRModels ;
		m_pcTNRModels = NULL ;
	}
}
void CTestScene::_loadModels()
{
	m_pcModels = new std::vector<CD3D9Model *> ;

	char szPath[] = "../Media/" ;
	std::vector<std::string> models ;
	//models.push_back("item_sap.RMD") ;
	//models.push_back("pang.RMD") ;
	models.push_back("manbung04.RMD");//models.push_back("manbung05.RMD") ;
	//models.push_back("building01.RMD") ;
	//models.push_back("arrow.RMD") ;
	models.push_back("redSphere.RMD");
	models.push_back("greenSphere.RMD");
	models.push_back("blueSphere.RMD");

	char str[256] ;
	CD3D9Model *model ;
	CASEData data ;

	std::vector<std::string>::iterator it ;
	for(it=models.begin(); it!=models.end(); it++)
	{
		sprintf(str, "%s%s", szPath, (*it).c_str()) ;
		data.ImportRMD(str) ;

		model = new CD3D9Model(m_pcRoot->getEnvironment()->getD3DDevice()) ;
		model->initialize(&data, m_pcTexContainer, m_pcShaderManager) ;

		m_pcModels->push_back(model) ;

		data.Release() ;
	}

}
void CTestScene::_loadTNRModels()
{
	CSecretTNRModel *pcTNRModel ;
	m_pcTNRModels = new std::vector<CSecretTNRModel *> ;
	Matrix4 matTransform ;

	std::vector<CD3D9Model *>::iterator it ;

	for(it=m_pcModels->begin(); it!=m_pcModels->end(); it++)
	{
		if(!strcmp((*it)->getModelName(), "manbung05"))
		{
			matTransform.Identity() ;

			CSecretMaterial *material = (*it)->getMaterial(0);
			material->m_fShine = 0.3f;
			material->m_fShineStrength = 1.21f;
		}
		else if (!strcmp((*it)->getModelName(), "redSphere") || !strcmp((*it)->getModelName(), "greenSphere") || !strcmp((*it)->getModelName(), "blueSphere")) {
			matTransform.SetScaling(0.25f, 0.25f, 0.25f);
		}
		else
		{
			matTransform.Identity() ;
		}

		pcTNRModel = new CSecretTNRModel(m_pcUniqueNumber) ;
		pcTNRModel->addModel((CSecretModel *)(*it), &matTransform, (*it)->getModelName()) ;
		m_pcTNRModels->push_back(pcTNRModel) ;
	}
}
bool CTestScene::Initialize()
{
	//test camera tracking
	CASEData data ;
	CASELoader loader ;
	loader.ReadASEFile("../Media/rotCamera2.ASE", &data) ;

	SAnimationInfo sAnimationInfo ;
	sAnimationInfo.fFirstFrame = data.m_sSceneInfo.fFirstFrame ;
	sAnimationInfo.fLastFrame = data.m_sSceneInfo.fLastFrame ;
	sAnimationInfo.fFrameSpeed = data.m_sSceneInfo.fFrameSpeed ;
	sAnimationInfo.fTicksPerFrame = data.m_sSceneInfo.fTicksPerFrame ;
	sAnimationInfo.fTotalFrame = sAnimationInfo.fLastFrame * sAnimationInfo.fTicksPerFrame ;

	m_pcCameraTracker = new CSecretCameraTracker() ;
	m_pcCameraTracker->initialize(data.m_apsCameraObject[0], &sAnimationInfo) ;

	CD3D9Environment *pcEnvironment = m_pcRoot->getEnvironment() ;
	
	m_pcTimer = m_pcRoot->m_pcTimer ;

	m_pcd3dFont = new CD3DFont( _T("Comic Sans MS"), 13, 0 ) ;
	if(m_pcd3dFont->InitDeviceObjects(pcEnvironment->getD3DDevice()) != S_OK)
		return false ;

	m_pcTexContainer = new CD3D9TextureContainer(m_pcRoot->getEnvironment()->getD3DDevice()) ;
	m_pcTexContainer->importFile("../Media/test3.txr") ;

	m_pcRenderingIngredient = m_pcRoot->getRenderingIngredient() ;
	m_pcShaderManager = m_pcRoot->getShaderManager() ;

	CHLSLLPPOpaque *opaque = (CHLSLLPPOpaque *)m_pcShaderManager->findShader("LPPopaque") ;
	//opaque->m_vOffset.x = 1.0f/(float)pcEnvironment->m_lScreenWidth ;
	//opaque->m_vOffset.y = 1.0f/(float)pcEnvironment->m_lScreenHeight ;

	//initialize light-pre pass renderer
	m_pcLPPRenderer = new CD3D9LPPRenderer() ;
	m_pcLPPRenderer->initialize(pcEnvironment, m_pcShaderManager) ;

	CHLSLSSAO* ssao =(CHLSLSSAO *)m_pcShaderManager->findShader("SSAO");
	ssao->m_vScreenSize = D3DXVECTOR2(pcEnvironment->m_lScreenWidth, pcEnvironment->m_lScreenHeight);
	ssao->m_vRandomSize = D3DXVECTOR2(64.0f, 64.0f);
	m_pcSSAO = new CD3D9PPSSAO();
	m_pcSSAO->initialize(pcEnvironment, ssao);


	CSecretLightProperty *lightproperty ;

	//set directional light
	//lightproperty = new CSecretLightProperty() ;
	//lightproperty->setDirectionalLight(Vector3(-1.0f, -1.0f, 0.0f), Vector3(8.0f, 8.0f, 4.0f)) ;
	//lightproperty->setDirectionalLight(Vector3(-1.0f, -1.0f, 0.0f), Vector3(245.0f, 203.0f, 74.0f)) ;
	//m_pcLPPRenderer->addLightProperty(lightproperty);

	//set point light
	lightproperty = new CSecretLightProperty("red") ;
	//lightproperty->setPointLight(Vector3(-10.0f, 8.0f, -10.0f), Vector3(241.0f, 224.0f, 35.0f), 12.0f) ;
	lightproperty->setPointLight(Vector3(-10.0f, 8.0f, -10.0f), Vector3(255.0f, 0.0f, 0.0f), 8.0f);
	m_pcLPPRenderer->addLightProperty(lightproperty) ;

	lightproperty = new CSecretLightProperty("green") ;
	lightproperty->setPointLight(Vector3(15.0f, 15.0f, -16.0f), Vector3(0.0f, 255.0f, 0.0f), 2.0f) ;
	m_pcLPPRenderer->addLightProperty(lightproperty) ;

	lightproperty = new CSecretLightProperty("blue") ;
	lightproperty->setPointLight(Vector3(10.0f, 5.0f, 0.0f), Vector3(0.0f, 0.0f, 255.0f), 2.0f) ;
	m_pcLPPRenderer->addLightProperty(lightproperty) ;

	//lightproperty = new CSecretLightProperty() ;
	//lightproperty->setPointLight(Vector3(0.0f, 2.0f, 14.0f), Vector3(232.0f, 32.0f, 230.0f), 12.0f) ;
	//m_pcLPPRenderer->addLightProperty(lightproperty) ;

	m_pcUniqueNumber = new CSecretUniqueNumber(1024) ;

	_loadModels() ;
	_loadTNRModels() ;

	m_pcCamera = new CSecretCamera() ;
	m_pcCamera->Initialize(Vector3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, 1.0f), Vector3(0.0f, 1.0f, 0.0f)) ;
	m_pcCamera->TransformbyQuat(0.0f, 0.0f, 0.0f, Vector3(0.0f, 0.0f, 0.0f)) ;

	m_cFrustum.initialize(pcEnvironment->getD3DDevice()) ;

	m_pcRenderingManager = new CD3D9RenderingManager() ;
	m_pcRenderingManager->initialize(pcEnvironment->getD3DDevice(), 128, m_pcCamera, &m_cFrustum, 64.0f) ;

	m_pcTimer->Process_real() ;

	RestoreDevice() ;

	m_sInput.bUpdateFrustum = true ;

	CVertexAnimationMesh::SVertex asVertices[4] ;
	CVertexAnimationMesh::SIndex asIndices[2] ;

	asVertices[0].pos = D3DXVECTOR3(10.0f, 10.0f, 10.0f) ;
	asVertices[0].tex = D3DXVECTOR2(0.0f, 0.0f) ;
	asVertices[1].pos = D3DXVECTOR3(20.0f, 10.0f, 10.0f) ;
	asVertices[1].tex = D3DXVECTOR2(1.0f, 0.0f) ;
	asVertices[2].pos = D3DXVECTOR3(10.0f,  0.0f, 10.0f) ;
	asVertices[2].tex = D3DXVECTOR2(0.0f, 1.0f) ;
	asVertices[3].pos = D3DXVECTOR3(20.0f,  0.0f, 10.0f) ;
	asVertices[3].tex = D3DXVECTOR2(1.0f, 1.0f) ;

	asIndices[0].index[0] = 0 ;
	asIndices[0].index[1] = 1 ;
	asIndices[0].index[2] = 2 ;

	asIndices[1].index[0] = 2 ;
	asIndices[1].index[1] = 1 ;
	asIndices[1].index[2] = 3 ;

	cVertexAni.initialize(pcEnvironment->getD3DDevice(), asVertices, 4, asIndices, 2, m_pcTexContainer->findD3DTexture("sol")) ;

	asVertices[0].pos = D3DXVECTOR3(10.0f, 10.0f, 0.0f) ;
	asVertices[1].pos = D3DXVECTOR3(20.0f, 10.0f, 0.0f) ;
	asVertices[2].pos = D3DXVECTOR3(10.0f,  0.0f, 0.0f) ;
	asVertices[3].pos = D3DXVECTOR3(20.0f,  0.0f, 0.0f) ;

	cVertexAni.setAnimation(10.0f, asVertices, 4, SinDampingAFunction) ;

	return true ;
}
long CTestScene::RestoreDevice()
{
	m_pcd3dFont->RestoreDeviceObjects() ;

	D3DXMATRIX matView, matProj ;

	D3DXVECTOR3 vPosition = D3DXVECTOR3(0.f, 5.0f, 0.0f) ;
	D3DXVECTOR3 vLookat = D3DXVECTOR3(0.f, 0.f, 1.f) ;
	D3DXVECTOR3 vUp = D3DXVECTOR3(0.f, 1.f, 0.f) ;
	D3DXMatrixLookAtLH(&matView, &vPosition, &vLookat, &vUp) ;

	CD3D9Environment *pcEnvironment = m_pcRoot->getEnvironment() ;

	m_pcRenderingIngredient->m_fNear = 0.1f ;
	m_pcRenderingIngredient->m_fFar = 256.0f ;
	D3DXMatrixPerspectiveFovLH(&matProj, deg_rad(45.0f), (float)pcEnvironment->m_lScreenWidth/(float)pcEnvironment->m_lScreenHeight, m_pcRenderingIngredient->m_fNear, m_pcRenderingIngredient->m_fFar) ;
	//D3DXMatrixPerspectiveFovLH(&matProj, deg_rad(45.0f), 4.0f/3.0f, 1.0f, 256.0f) ;
	m_pcRenderingIngredient->m_matProj = matProj ;


	//set light perspective matrix
	//D3DXMatrixPerspectiveFovLH(&matProj, deg_rad(45.0f), 1.0f, 1.0f, 1024.0f) ;
	D3DXMatrixOrthoLH(&matProj, 512.0f, 512.0f, 1.0f, 512.0f) ;

	LPDIRECT3DDEVICE9 pd3dDevice = pcEnvironment->getD3DDevice() ;
	pd3dDevice->SetRenderState(D3DRS_ZENABLE, TRUE) ;
	pd3dDevice->SetRenderState(D3DRS_LIGHTING, FALSE) ;

	return S_OK ;
}
void CTestScene::PreProcess()
{
}
CD3D9LPPRenderer *CTestScene::getLPPRender()
{
	return m_pcLPPRenderer ;
}
void CTestScene::findParticularShader(void *pcScene, CSecretMesh *pcMesh, CSecretShader **ppShader, int *pnPass)
{
	CTestScene *pcTestScene = (CTestScene *)pcScene ;
	pcTestScene->getLPPRender()->findGBufferShader(pcMesh->getKind(), ppShader, pnPass) ;
}
void CTestScene::Process()
{
	int i ;
	m_fTimeSinceLastFrame = m_pcTimer->Process_real() ;
	m_nfps = m_pcTimer->GetAvgFrame() ;

	ProcessInputDevice(m_fTimeSinceLastFrame) ;

	cVertexAni.process(0.001f) ;

	m_pcRenderingManager->reset() ;
	std::vector<CSecretTNRModel *>::iterator it ;
	for(it=m_pcTNRModels->begin(); it!=m_pcTNRModels->end(); it++)
	{
		if(isRenderOption(LPP_RENDERING))
		{
			//메쉬 하나당 지버퍼셰이더 셋팅
			CSecretModel *pcModel = (*it)->getCurrentModel() ;
			for(i=0 ; i<pcModel->getNumNode() ; i++)
			{
				CSecretShader *shader=NULL ;
				int pass ;
				CSecretMesh *pcMesh = (CSecretMesh *)pcModel->getNode(i) ;
				m_pcLPPRenderer->findGBufferShader(pcMesh->getKind(), &shader, &pass) ;
				pcMesh->m_pcShader = shader ;
				pcMesh->m_nPass = pass ;
			}
		}//LPP_RENDERING

		m_pcRenderingManager->addModel((*it)) ;
	}

	CSecretTNRModel *redSphere = NULL, *greenSphere = NULL, *blueSphere=NULL;
	for (std::vector<CSecretTNRModel *>::iterator it = m_pcTNRModels->begin(); it != m_pcTNRModels->end(); it++) {
		if (!strcmp((*it)->getTNRName(), "redSphere")) {
			redSphere = (*it);
		}
		else if (!strcmp((*it)->getTNRName(), "greenSphere")) {
			greenSphere = (*it);
		}
		else if (!strcmp((*it)->getTNRName(), "blueSphere")) {
			blueSphere = (*it);
		}
		m_pcRenderingManager->addModel((*it));
	}

	std::vector<CSecretLightProperty *> *lightProperites = m_pcLPPRenderer->getLightProperites();
	for (std::vector<CSecretLightProperty *>::iterator it = lightProperites->begin(); it != lightProperites->end(); it++) {
		CSecretLightProperty *light = (*it);
		if (light->m_nType == CSecretLightProperty::LIGHTTYPE_POINT) {
			Vector3 v = light->getLightPos()->Normalize();
			float theta = calculateAngle(v.x, -v.z);
			//TRACE("theta = %g\n", rad_deg(theta));
			Matrix4 matRot;
			matRot.SetRotatebyAxisY(deg_rad(30*m_fTimeSinceLastFrame), true);
			Vector3 rotV = v * matRot;
			//TRACE("rotV(%g %g %g)\n", enumVector(rotV));
			float includedAngle = IncludedAngle(v, rotV);
			//TRACE("includedAngle = %g\n", rad_deg(includedAngle));
			light->m_vLightPos = rotV * light->getLightPos()->Magnitude();

			std::function<void(CSecretTNRModel *, Vector3 &)> setLightSphere = [](CSecretTNRModel *tnrModel, Vector3 &pos) {
				Matrix4 matTransform;
				matTransform.SetTranslation(pos);
				matTransform = tnrModel->getTransform()->DecomposeScaling() * matTransform;
				tnrModel->setTransform(&matTransform);
			};

			if (redSphere && strcmp((*it)->m_szName, "red") == 0) {
				setLightSphere(redSphere, light->m_vLightPos);
			}
			else if (greenSphere && strcmp((*it)->m_szName, "green") == 0) {
				setLightSphere(greenSphere, light->m_vLightPos);
			}
			else if (greenSphere && strcmp((*it)->m_szName, "blue") == 0) {
				setLightSphere(blueSphere, light->m_vLightPos);
			}
		}
	}

	m_pcRenderingManager->process() ;
}
void CTestScene::PostProcess()
{
}
void CTestScene::_render_forward()
{
	D3DXMATRIX matWorld, matView ;

	D3DXMatrixIdentity(&matWorld) ;

	Vector3 lookat, pos ;
	m_pcCameraTracker->tracking(0.0025f, lookat, pos) ;
	//m_pcCamera->BuildViewMatrix(lookat, pos) ;

	COPY_D3DXMATRIX(matView, (*m_pcCamera->GetMatrix())) ;
	m_pcRenderingIngredient->m_matView = matView ;
	COPY_VECTOR3(m_pcRenderingIngredient->m_vCameraPos, m_pcCamera->GetPosition()) ;

	if(m_sInput.bUpdateFrustum)
	{
		m_cFrustum.buildFrustum(&matView, &m_pcRenderingIngredient->m_matProj) ;
		//m_sInput.bUpdateFrustum = false ;
	}

	LPDIRECT3DDEVICE9 pd3dDevice = m_pcRoot->getEnvironment()->getD3DDevice() ;

	pd3dDevice->Clear(0L, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, 0x008fc3fa, 1.0f, 0L) ;
	if(SUCCEEDED(pd3dDevice->BeginScene()))
	{
		pd3dDevice->SetTransform(D3DTS_WORLD, &matWorld) ;
		pd3dDevice->SetTransform(D3DTS_VIEW, &matView) ;
		pd3dDevice->SetTransform(D3DTS_PROJECTION, &m_pcRenderingIngredient->m_matProj) ;

		Matrix4 matTransform ;

		int nDistQ=0 ;
		D3DXVECTOR3 eye(0.0f, 220.0f, -220.1f), lookat(0.0f, 0.0f, 0.0f), up(0.0f, 1.0f, 0.0f) ;

		std::vector<CD3D9Model *>::iterator it ;

		//model render
		pd3dDevice->SetTransform(D3DTS_VIEW, &matView) ;
		pd3dDevice->SetTransform(D3DTS_PROJECTION, &m_pcRenderingIngredient->m_matProj) ;

		for (std::vector<CSecretTNRModel *>::iterator it = m_pcTNRModels->begin(); it != m_pcTNRModels->end(); it++) {
			CD3D9Model *model = (CD3D9Model *)(*it)->getCurrentModel();
			for (int i = 0; i < model->getNumNode(); i++)
			{
				model->render(model->getNode(i), (*it)->getTransform(), m_pcShaderManager->getShader(0), 0);
			}
		}

		cVertexAni.render() ;

		RenderText() ;
	}

	pd3dDevice->EndScene() ;
	pd3dDevice->Present(NULL, NULL, NULL, NULL) ;
}
void CTestScene::_render_lpp()
{
	D3DXMATRIX matWorld, matView ;

	D3DXMatrixIdentity(&matWorld) ;

	Vector3 lookat, pos ;
	m_pcCameraTracker->tracking(0.0025f, lookat, pos) ;
	//m_pcCamera->BuildViewMatrix(lookat, pos) ;

	COPY_D3DXMATRIX(matView, (*m_pcCamera->GetMatrix())) ;
	m_pcRenderingIngredient->m_matView = matView ;
	COPY_VECTOR3(m_pcRenderingIngredient->m_vCameraPos, m_pcCamera->GetPosition()) ;

	if(m_sInput.bUpdateFrustum)
	{
		m_cFrustum.buildFrustum(&matView, &m_pcRenderingIngredient->m_matProj) ;
		//m_sInput.bUpdateFrustum = false ;
	}
	
	LPDIRECT3DDEVICE9 pd3dDevice = m_pcRoot->getEnvironment()->getD3DDevice() ;

	pd3dDevice->Clear(0L, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, 0x008fc3fa, 1.0f, 0L) ;
	if(SUCCEEDED(pd3dDevice->BeginScene()))
	{
		pd3dDevice->SetTransform(D3DTS_WORLD, &matWorld) ;
		pd3dDevice->SetTransform(D3DTS_VIEW, &matView) ;
		pd3dDevice->SetTransform(D3DTS_PROJECTION, &m_pcRenderingIngredient->m_matProj) ;

		Matrix4 matTransform ;

		int nDistQ=1.0f ;
		D3DXVECTOR3 eye(0.0f, 220.0f, -220.0f), lookat(0.0f, 0.0f, 0.0f), up(0.0f, 1.0f, 0.0f) ;

		//model render
		pd3dDevice->SetTransform(D3DTS_VIEW, &matView) ;
		pd3dDevice->SetTransform(D3DTS_PROJECTION, &m_pcRenderingIngredient->m_matProj) ;

		//render gbuffer
		m_pcLPPRenderer->storeBackBuffer() ;
		m_pcLPPRenderer->setGBuffer() ;

		for (std::vector<CSecretTNRModel *>::iterator it = m_pcTNRModels->begin(); it != m_pcTNRModels->end(); it++) {
			CD3D9Model *model= (CD3D9Model *)(*it)->getCurrentModel();
			if (!strcmp(model->getModelName(), "arrow")) {
				Matrix4 matR;
				matR.SetRotatebyAxisX(90.0f);
				//Matrix4 transform = matR * (*pmatCameraTrack) ;
				for (int i = 0; i < model->getNumNode(); i++)
				{
					CSecretNode *node = model->getNode(i);
					CSecretShader *shader = NULL;
					int pass;
					m_pcLPPRenderer->findGBufferShader(node->getKind(), &shader, &pass);
					model->render(node, &matTransform, shader, pass);
				}
			}
			else {
				for (int i = 0; i < model->getNumNode(); i++)
				{
					CSecretNode *node = model->getNode(i);
					CSecretShader *shader = NULL;
					int pass;
					m_pcLPPRenderer->findGBufferShader(node->getKind(), &shader, &pass);
					
					model->render(node, (*it)->getTransform(), shader, pass);
				}
			}
		}

		m_pcLPPRenderer->restoreBackBuffer() ;

		//render lbuffer
		m_pcLPPRenderer->render() ;

		//m_pcLPPRenderer->renderQuad(0, 0, 1280, 720) ;
		if (m_pcSSAO)
		{
			m_pcSSAO->m_pTexNormal = m_pcLPPRenderer->getGBufferMRTs()->getRenderTarget(0)->m_pTex;
			m_pcSSAO->m_pTexDepth = m_pcLPPRenderer->getGBufferMRTs()->getRenderTarget(1)->m_pTex;
			m_pcSSAO->m_pTexRandom = m_pcTexContainer->findD3DTexture("random");

			CHLSLSSAO *ssao = (CHLSLSSAO *)m_pcSSAO->getShader();

			D3DXMATRIX matInvProj;
			D3DXMatrixInverse(&matInvProj, NULL, &m_pcRenderingIngredient->m_matProj);
			D3DXMATRIX matVP = m_pcRenderingIngredient->m_matView * m_pcRenderingIngredient->m_matProj;
			D3DXMatrixInverse(&ssao->m_matInvVP, NULL, &matVP);
			ssao->m_matView = m_pcRenderingIngredient->m_matView;

			m_pcSSAO->setMatInvProj(&matInvProj);
			m_pcSSAO->render();
		}

		CHLSLLPPOpaque *opaque = (CHLSLLPPOpaque *)m_pcShaderManager->findShader("LPPopaque") ;
		CHLSLRigid *rigid = (CHLSLRigid *)m_pcShaderManager->findShader("rigid");
		opaque->m_pTexLight = m_pcLPPRenderer->getCurrentLBuffer()->getRenderTarget()->m_pTex ;
		opaque->m_pTexSSAO = m_pcSSAO->getRenderTarget()->m_pTex;

		pd3dDevice->Clear(0L, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, 0x008fc3fa, 1.0f, 0L) ;

		for (std::vector<CSecretTNRModel *>::iterator it = m_pcTNRModels->begin(); it != m_pcTNRModels->end(); it++) {
			CD3D9Model *model = (CD3D9Model *)(*it)->getCurrentModel();
			if (!strcmp(model->getModelName(), "arrow")) {
				Matrix4 matR;
				matR.SetRotatebyAxisX(90.0f);
				//Matrix4 transform = matR * (*pmatCameraTrack) ;

				for (int i = 0; i < model->getNumNode(); i++)
				{
					model->render(model->getNode(i), (*it)->getTransform(), opaque, 0);
				}
			}
			else {
				for (int i = 0; i < model->getNumNode(); i++)
				{
					model->render(model->getNode(i), (*it)->getTransform(), opaque, 0);
				}
			}
		}

		int xoffset = 0;
		int size = 200;
		m_pcLPPRenderer->getGBufferMRTs()->renderQuad(0, xoffset, 0, size, size);
		xoffset += size;
		m_pcLPPRenderer->getGBufferMRTs()->renderQuad(1, xoffset, 0, size, size);
		xoffset += size;
		m_pcLPPRenderer->getGBufferMRTs()->renderQuad(2, xoffset, 0, size, size);

		//if (m_pcSSAO) {
		//	m_pcSSAO->getRenderTarget()->renderQuad(0, 0, 1280, 760);
		//}

		//m_pcLPPRenderer->getCurrentLBuffer()->getRenderTarget()->renderQuad(0, 0, m_pcRoot->getEnvironment()->m_lScreenWidth, m_pcRoot->getEnvironment()->m_lScreenHeight);
		//m_pcLPPRenderer->getGBufferMRTs()->renderQuad(2, xoffset, 0, m_pcRoot->getEnvironment()->m_lScreenWidth, m_pcRoot->getEnvironment()->m_lScreenHeight);
		//xoffset += 320;
		//m_pcLPPRenderer->getGBufferMRTs()->renderQuad(0, xoffset, 0, 320, 240);
		//xoffset += 320;
		//m_pcLPPRenderer->getGBufferMRTs()->renderQuad(2, xoffset, 0, 320, 240);

		//m_pcLPPRenderer->getGBufferMRTs()->renderQuad(2, 0, 0, 120, 120);
		//m_pcLPPRenderer->renderQuad(0, 0, 640, 480) ;
		//m_pcLPPRenderer->renderQuad(0, 0, 320, 240);

		if(isRenderOption(FRUSTUM_RENDERING))
			m_cFrustum.render(0xffffffff) ;

		RenderText() ;
	}

	pd3dDevice->EndScene() ;
	pd3dDevice->Present(NULL, NULL, NULL, NULL) ;
}
void CTestScene::Render()
{
	if(isRenderOption(LPP_RENDERING))
		_render_lpp() ;
	else
		_render_forward() ;
}
void CTestScene::RenderText()
{
	if(!m_pcd3dFont)
		return ;

	CD3D9Environment *pcEnvironment = m_pcRoot->getEnvironment();

	char szMsg[256] ;
	D3DCOLOR FontColor = D3DCOLOR_ARGB(255,255,0,0);
	//float fDrawLine=380.0f, fTextHeight=20.0f, fTextRightSpace=2.0f ;
	float fTextRightSpace = 2.0f;
	float fTextHeight = -20.0f;
	float fDrawLine = pcEnvironment->m_lScreenHeight + fTextHeight;

	CHLSLLPPOpaque *opaque = (CHLSLLPPOpaque *)m_pcShaderManager->findShader("LPPopaque");
	sprintf(szMsg, "opaque Offset(%g, %g)", opaque->m_vOffset.x, opaque->m_vOffset.y);
	m_pcd3dFont->DrawText(fTextRightSpace, fDrawLine += fTextHeight, FontColor, szMsg);

	sprintf(szMsg, "camera pos(%g, %g, %g)", enumVector(m_pcCamera->GetPosition()));
	m_pcd3dFont->DrawText(fTextRightSpace, fDrawLine += fTextHeight, FontColor, szMsg);

	sprintf(szMsg, "FPS=%d TimeSinceLastFrame=%g", m_nfps, m_fTimeSinceLastFrame) ;
	m_pcd3dFont->DrawText(fTextRightSpace, fDrawLine+=fTextHeight, FontColor, szMsg) ;

	//sprintf(szMsg, "m_fShadowMapBias=%g)", m_fShadowMapBias) ;
	//m_pcd3dFont->DrawText(fTextRightSpace, fDrawLine+=fTextHeight, FontColor, szMsg) ;

	//CHLSLLBuffer *lbuffer = (CHLSLLBuffer *)m_pcLBuffer->getShader() ;
	//sprintf(szMsg, "pointLight (%g %g %g)", enumVector(lbuffer->m_vLightPos)) ;
	//m_pcd3dFont->DrawText(fTextRightSpace, fDrawLine+=fTextHeight, FontColor, szMsg) ;

	//CHLSLSSAO *ssao = (CHLSLSSAO *)m_pcSSAO->getShader() ;
	//if(ssao)
	//{
	//	sprintf(szMsg, "sample_rad=%g", ssao->m_fSample_Rad) ;
	//	m_pcd3dFont->DrawText(fTextRightSpace, fDrawLine+=fTextHeight, FontColor, szMsg) ;

	//	sprintf(szMsg, "scale=%g", ssao->m_fScale) ;
	//	m_pcd3dFont->DrawText(fTextRightSpace, fDrawLine+=fTextHeight, FontColor, szMsg) ;

	//	sprintf(szMsg, "bias=%g", ssao->m_fBias) ;
	//	m_pcd3dFont->DrawText(fTextRightSpace, fDrawLine+=fTextHeight, FontColor, szMsg) ;

	//	sprintf(szMsg, "intensity=%g", ssao->m_fIntensity) ;
	//	m_pcd3dFont->DrawText(fTextRightSpace, fDrawLine+=fTextHeight, FontColor, szMsg) ;
	//}
}
void CTestScene::ProcessInputDevice(float fTimeSinceLastFrame)
{
	int nKeyStatus = m_pcRoot->m_pcInput->getKeyStatus() ;
	int nKey = m_pcRoot->m_pcInput->getKeyEvent() ;

	Vector3 p ;
	float fPitch=0.0f, fYaw=0.0f, fRoll=0.0f ;
	float move = 60.5f * fTimeSinceLastFrame ;

	if(ScanKeyCode('W'))
	{
		p.z += move ;
	}
	if(ScanKeyCode('S'))
	{
		p.z -= move ;
	}
	if(ScanKeyCode('A'))
	{
		p.x -= move ;
	}
	if(ScanKeyCode('D'))
	{
		p.x += move ;
	}
	if(ScanKeyCode('Q'))
	{
		p.y += move ;
	}
	if(ScanKeyCode('E'))
	{
		p.y -= move ;
	}

	if(ScanKeyCode(VK_LEFT))
	{
		fYaw -= move ;
	}
	if(ScanKeyCode(VK_RIGHT))
	{
		fYaw += move ;
	}
	if(ScanKeyCode(VK_UP))
	{
		fPitch -= move ;
	}
	if(ScanKeyCode(VK_DOWN))
	{
		fPitch += move ;
	}

	CD3D9Environment *pcEnvironment = m_pcRoot->getEnvironment() ;
	CHLSLLPPOpaque *opaque = (CHLSLLPPOpaque *)m_pcShaderManager->findShader("LPPopaque") ;
	static D3DXVECTOR2 vOffset = D3DXVECTOR2(1.0f, 1.0f) ;
	float scale = 1.0f;
	
	if(ScanKeyCode('U'))
	{
		vOffset.x += fTimeSinceLastFrame * scale;
		opaque->m_vOffset.x = vOffset.x/1280.0f ;
	}
	else if(ScanKeyCode('J'))
	{
		vOffset.x -= fTimeSinceLastFrame * scale;
		opaque->m_vOffset.x = vOffset.x/1280.0f ;
	}
	if(ScanKeyCode('I'))
	{
		vOffset.y += fTimeSinceLastFrame * scale;
		opaque->m_vOffset.y = vOffset.y/720.0f ;

		//m_fShadowMapBias += fTimeSinceLastFrame*0.01f ;
	}
	else if(ScanKeyCode('K'))
	{
		vOffset.y -= fTimeSinceLastFrame * scale; ;
		opaque->m_vOffset.y = vOffset.y/720.0f ;

		//m_fShadowMapBias -= fTimeSinceLastFrame*0.01f ;
	}

	if(nKey == VK_SPACE)
	{
		m_sInput.bUpdateFrustum = true ;
	}

	//screen space ambient occlusion parameters
	//m_pcSSAO->getShader()->tuneParameters(fTimeSinceLastFrame) ;

	//m_pcLBuffer->getShader()->tuneParameters(fTimeSinceLastFrame) ;

	m_pcCamera->TransformbyQuat(fPitch, fYaw, fRoll, p) ;

	if(p.Magnitude() > 0.0f)
	{
		TRACE("camera pos (%g, %g, %g) lookat(%g, %g, %g)\r\n",
			enumVector(m_pcCamera->GetPosition()), enumVector(m_pcCamera->GetLookat())) ;
	}

	m_pcRoot->m_pcInput->reset() ;
}
bool CTestScene::isRenderOption(int nOption)
{
	if(m_nRenderOption & nOption)
		return true ;
	return false ;
}
