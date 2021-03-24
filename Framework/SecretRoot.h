#pragma once

#include "SecretObject.h"
#include "SecretScene.h"
#include <string>
#include <map>

class CD3D9Environment ;
class CRenderingIngredient ;
class CD3D9ShaderManager ;
class CSecretCamera ;
class CSecretInput ;
class CSecretTimer ;

//현재 어플리케이션의 총 장면수
#define MAXNUM_SCENE 12

//루트는 모든 장면을 관리하는 클래스다.
class CSecretRoot : public CSecretObject
{
public :
	enum SCENE
	{
		//SCENE_GAMETITLE=0,
		SCENE_TESTSCENE=0,
	} ;

	enum SECRETROOTSTATUS
	{
		ROOTSTA_SCENEINIT=0, ROOTSTA_SCENEPREPROC, ROOTSTA_SCENEPROC, ROOTSTA_SCENEPOSTPROC, ROOTSTA_SCENECHANGE
	} ;

//Member Variable
//private :
	int m_nSceneStatus ;//장면상태
	int m_nCurScene ;
	int m_nNextScene ;

	CSecretScene *m_apcScene[MAXNUM_SCENE] ;//장면의 포인터배열
	CSecretScene *m_pcCurScene ;//현재 돌아가고 있는 장면

public :
	bool m_bTerminator ;
	CSecretInput *m_pcInput ;
	CSecretTimer *m_pcTimer ;

//Member Function
private :

	CD3D9Environment *m_pcEnvironment ;
	CRenderingIngredient *m_pcRenderingIngredient ;
	CD3D9ShaderManager *m_pcShaderManager ;
	CSecretCamera *m_pcCamera ;

	void _initShader() ;

public :

public :
	DeclareDynamic(CSecretRoot)
    DeclareDynCreate(CSecretRoot)

	CSecretRoot() ;
	virtual ~CSecretRoot() ;

    bool Initialize(CD3D9Environment *pcEnvironment) ;
	void Process() ;
	void Render() ;
	void Release() ;

	void InitShader() ;

	void SetSceneStatus(int nStatus) ;
	unsigned long GetSceneStatus() ;

	CSecretScene *getCurrentScene() ;

	void	SettingResourcePath();

	CD3D9Environment *getEnvironment() ;
	CRenderingIngredient *getRenderingIngredient() ;
	CD3D9ShaderManager *getShaderManager() ;
	CSecretCamera *getCamera() ;
} ;
