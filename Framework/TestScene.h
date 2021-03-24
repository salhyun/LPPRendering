#pragma once

#include "SecretScene.h"
#include <vector>

#include "D3D9Include.h"
#include "Secret.h"

//class CD3D9TextureContainer ;
//class CRenderingIngredient ;
//class CD3D9ShaderManager ;
//class CSecretCamera ;
//class CD3D9Model ;
//class CSecretTNRModel ;
//class CD3D9RenderTarget ;
//class CD3D9MultipleRenderTargets ;
class CD3D9PPSSAO;
class CPPLBuffer ;
class CD3DFont ;
class CSecretTimer ;

class CTestScene : public CSecretScene
{
public :
	enum SCENESTATUS { SCENESTA_LOADING=0, SCENESTA_PLAYING } ;
	enum GAMESTATUS { GAMESTA_VIEWHOUSE=0, GAMESTA_THROWING, GAMESTA_MARKING, GAMESTA_CHANGEPLAYER, GAMESTA_RESULT } ;

	enum CAMERAMODE { CAMERAMODE_FREE=0, CAMERAMODE_GAME } ;

	enum RENDEROPTION
	{
		FORWARD_RENDERING=0x0,
		LPP_RENDERING=0x01,
		TRAPEZOIDALSHADOWMAP=0x02,
		FRUSTUM_RENDERING=0x04
	} ;

	struct SInput : public SSceneInput
	{
		bool bDrawFrustum ;
		bool bUpdateFrustum ;

		SInput() ;
		virtual ~SInput() ;
	} ;

protected :
	int m_nRenderOption ;
	int m_nGameStatus ;

	CD3D9TextureContainer *m_pcTexContainer ;

	std::vector<CD3D9Model *> *m_pcModels ;
	std::vector<CSecretTNRModel *> *m_pcTNRModels ;

	CD3D9RenderingManager *m_pcRenderingManager ;

	CRenderingIngredient *m_pcRenderingIngredient ;
	CD3D9ShaderManager *m_pcShaderManager ;
	CSecretCamera *m_pcCamera ;

	CD3D9LPPRenderer *m_pcLPPRenderer ;
	CD3D9PPSSAO *m_pcSSAO;

	CD3DFont *m_pcd3dFont ;
	CSecretTimer *m_pcTimer ;

	CSecretCameraTracker *m_pcCameraTracker ;

	CD3D9Frustum m_cFrustum ;

	float m_fShadowMapBias ;

	SInput m_sInput ;

	void _loadModels() ;
	void _loadTNRModels() ;

	void _render_forward() ;
	void _render_lpp() ;
public :

	DeclareDynamic(CTestScene)
	DeclareDynCreate(CTestScene)

	CTestScene() ;
	virtual ~CTestScene() ;

	static void findParticularShader(void *pcScene, CSecretMesh *pcMesh, CSecretShader **ppShader, int *pnPass) ;

	virtual bool Initialize() ;
	virtual void PreProcess() ;
	virtual void Process() ;
	virtual void PostProcess() ;
	virtual void Render() ;
	virtual void RenderText() ;
	virtual void Release() ;
	virtual long RestoreDevice() ;

	virtual void ProcessInputDevice(float fTimeSinceLastFrame) ;

	CD3D9LPPRenderer *getLPPRender() ;

	bool isRenderOption(int nOption) ;
} ;