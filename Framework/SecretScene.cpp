#include "SecretScene.h"
#include "SecretRoot.h"
#include "Secret.h"

ImplementDynamic(CSecretScene)
ImplementDynCreate(CSecretScene)

SSceneInput::SSceneInput()
{
	fPitch = 0.0f ;
	fYaw = 0.0f ;
	fRoll = 0.0f ;
	fLeftRight = 0.0f ;
	fUpDown = 0.0f ;
	fZoom = 0.0f ;
}
SSceneInput::~SSceneInput()
{
}

CSecretScene::CSecretScene()
{
	m_pcUniqueNumber = NULL ;
}

CSecretScene::~CSecretScene()
{
	if(m_pcUniqueNumber)
	{
		delete m_pcUniqueNumber ;
		m_pcUniqueNumber = NULL ;
	}
}

bool CSecretScene::Initialize()
{
	return true ;
}

void CSecretScene::PreProcess()
{
}

void CSecretScene::Process()
{
}

void CSecretScene::PostProcess()
{
}

void CSecretScene::Render()
{
}

void CSecretScene::RenderText()
{
}

void CSecretScene::Release()
{
}
long CSecretScene::RestoreDevice()
{
	return 0 ;
}
void CSecretScene::ProcessInputDevice(float fElaspedTime)
{
}
float CSecretScene::GetTimeSinceLastFrame()
{
	return m_fTimeSinceLastFrame ;
}
char *CSecretScene::GetSceneName()
{
	return m_szSceneName ;
}
void CSecretScene::setRoot(CSecretRoot *pcRoot)
{
	m_pcRoot = pcRoot ;
}
